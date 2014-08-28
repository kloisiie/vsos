/*
 * \brief   Libc plugin for accessing a file-system session
 * \author  Norman Feske
 * \date    2012-04-11
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/env.h>
#include <base/printf.h>
#include <file_system_session/connection.h>
#include <os/path.h>

/* libc includes */
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

/* libc plugin interface */
#include <libc-plugin/plugin.h>
#include <libc-plugin/fd_alloc.h>

/* libc-internal includes */
#include <libc_mem_alloc.h>

static bool const verbose = false;


namespace File_system { struct Packet_ref { }; }


namespace {

enum { PATH_MAX_LEN = 256 };

typedef Genode::Path<PATH_MAX_LEN> Canonical_path;


static File_system::Session *file_system()
{
	static Genode::Allocator_avl tx_buffer_alloc(Genode::env()->heap());
	static File_system::Connection fs(tx_buffer_alloc);
	return &fs;
}


struct Node_handle_guard
{
	File_system::Node_handle handle;

	Node_handle_guard(File_system::Node_handle handle) : handle(handle) { }

	~Node_handle_guard() { file_system()->close(handle); }
};


class Plugin_context : public Libc::Plugin_context,
                       public File_system::Packet_ref
{
	private:

		enum Type { TYPE_FILE, TYPE_DIR, TYPE_SYMLINK };

		Type _type;

		File_system::Node_handle _node_handle;

		int _fd_flags;
		int _status_flags;

		/**
		 * Current file position if manually seeked, or ~0 for append mode
		 */
		off_t _seek_offset;

	public:

		bool in_flight;

		Plugin_context(File_system::File_handle handle)
		: _type(TYPE_FILE), _node_handle(handle), _fd_flags(0),
		  _status_flags(0), _seek_offset(~0), in_flight(false) { }

		Plugin_context(File_system::Dir_handle handle)
		: _type(TYPE_DIR), _node_handle(handle), _fd_flags(0),
		  _status_flags(0), _seek_offset(0), in_flight(false) { }

		Plugin_context(File_system::Symlink_handle handle)
		: _type(TYPE_SYMLINK), _node_handle(handle), _fd_flags(0),
		  _status_flags(0), _seek_offset(~0), in_flight(false) { }

		File_system::Node_handle node_handle() const { return _node_handle; }

		/**
		 * Set/get file descriptor flags
		 */
		void fd_flags(int flags) { _fd_flags = flags; }
		int fd_flags() { return _fd_flags; }

		/**
		 * Set/get file status status flags
		 */
		void status_flags(int flags) { _status_flags = flags; }
		int status_flags() { return _status_flags; }

		/**
		 * Return true of handle is append mode
		 */
		bool is_appending() const { return ~0 == _seek_offset; }

		/**
		 * Set seek offset, switch to non-append mode
		 */
		void seek_offset(size_t seek_offset) { _seek_offset = seek_offset; }

		/**
		 * Return seek offset if handle is in non-append mode
		 */
		off_t seek_offset() const { return _seek_offset; }

		/**
		 * Advance current seek position by 'incr' number of bytes
		 *
		 * This function has no effect if the handle is in append mode.
		 */
		void advance_seek_offset(size_t incr)
		{
			if (!is_appending())
				_seek_offset += incr;
		}

		void infinite_seek_offset()
		{
			_seek_offset = ~0;
		}

		virtual ~Plugin_context() { }
};


static inline Plugin_context *context(Libc::File_descriptor *fd)
{
	return fd->context ? static_cast<Plugin_context *>(fd->context) : 0;
}


static void wait_for_acknowledgement(File_system::Session::Tx::Source &source)
{
	::File_system::Packet_descriptor packet = source.get_acked_packet();
	
	if (verbose)
		PDBG("got acknowledgement for packet of size %zd", packet.size());

	static_cast<Plugin_context *>(packet.ref())->in_flight = false;

	source.release_packet(packet);
}


/**
 * Collect pending packet acknowledgements, freeing the space occupied
 * by the packet in the bulk buffer
 *
 * This function should be called prior enqueing new packets into the
 * packet stream to free up space in the bulk buffer.
 */
static void collect_acknowledgements(File_system::Session::Tx::Source &source)
{
	while (source.ack_avail())
		wait_for_acknowledgement(source);
}


static void obtain_stat_for_node(File_system::Node_handle node_handle,
                                 struct stat *buf)
{
	if (!buf)
		return;

	File_system::Status status = file_system()->status(node_handle);

	/*
	 * Translate status information to 'struct stat' format
	 */
	memset(buf, 0, sizeof(struct stat));
	buf->st_size = status.size;

	if (status.is_directory())
		buf->st_mode |= S_IFDIR;
	else if (status.is_symlink())
		buf->st_mode |= S_IFLNK;
	else
		buf->st_mode |= S_IFREG;

	struct tm tm;
	memset(&tm, 0, sizeof(struct tm));

	buf->st_mtime = mktime(&tm);

	if (buf->st_mtime == -1 && verbose)
		PERR("mktime() returned -1, the file modification time reported by stat() will be incorrect");
}


class Plugin : public Libc::Plugin
{
	private:

		::off_t _file_size(Libc::File_descriptor *fd)
		{
			struct stat stat_buf;
			if (fstat(fd, &stat_buf) == -1)
				return -1;
			return stat_buf.st_size;
		}

	public:

		/**
		 * Constructor
		 */
		Plugin() { }

		~Plugin() { }

		bool supports_mkdir(const char *path, mode_t)
		{
			if (verbose)
				PDBG("path = %s", path);
			return true;
		}

		bool supports_open(const char *pathname, int flags)
		{
			if (verbose)
				PDBG("pathname = %s", pathname);
			return true;
		}

		bool supports_readlink(const char *path, char *, size_t)
		{
			if (verbose)
				PDBG("path = %s", path);
			return true;
		}

		bool supports_rename(const char *oldpath, const char *newpath)
		{
			if (verbose)
				PDBG("oldpath = %s, newpath = %s", oldpath, newpath);
			return true;
		}

		bool supports_stat(const char *path)
		{
			if (verbose)
				PDBG("path = %s", path);
			return true;
		}

		bool supports_symlink(const char *oldpath, const char *newpath)
		{
			if (verbose)
				PDBG("oldpath = %s, newpath = %s", oldpath, newpath);
			return true;
		}

		bool supports_unlink(const char *path)
		{
			if (verbose)
				PDBG("path = %s", path);
			return true;
		}

		bool supports_mmap() { return true; }

		int close(Libc::File_descriptor *fd)
		{
			/* wait for the completion of all operations of the context */
			while (context(fd)->in_flight) {
				if (verbose)
					PDBG("wait_for_acknowledgement");
				wait_for_acknowledgement(*file_system()->tx());
			}

			file_system()->close(context(fd)->node_handle());

			Genode::destroy(Genode::env()->heap(), context(fd));
			Libc::file_descriptor_allocator()->free(fd);

			return 0;
		}

		int fcntl(Libc::File_descriptor *fd, int cmd, long arg)
		{
			switch (cmd) {
				case F_GETFD: return context(fd)->fd_flags();
				case F_SETFD: context(fd)->fd_flags(arg); return 0;
				case F_GETFL: return context(fd)->status_flags();
				default: PERR("fcntl(): command %d not supported", cmd); return -1;
			}
		}

		int fstat(Libc::File_descriptor *fd, struct stat *buf)
		{
			try {
				obtain_stat_for_node(context(fd)->node_handle(), buf);
				return 0;
			}
			catch (...) {
				struct Unhandled_exception_in_fstat { };
				throw Unhandled_exception_in_fstat();
			}
			return -1;
		}

		int fstatfs(Libc::File_descriptor *, struct statfs *buf)
		{
			/* libc's opendir() fails if _fstatfs() returns -1, so we return 0 here */
			if (verbose)
				PDBG("_fstatfs() called - not yet implemented");
			return 0;
		}

		int fsync(Libc::File_descriptor *fd)
		{
			if (verbose)
				PDBG("not implemented");
			return -1;
		}

		int ftruncate(Libc::File_descriptor *fd, ::off_t length)
		{
			File_system::Node_handle node_handle = context(fd)->node_handle();
			File_system::File_handle &file_handle =
			    static_cast<File_system::File_handle&>(node_handle);

			try {
				file_system()->truncate(file_handle, length);
			} catch (File_system::Invalid_handle) {
				errno = EINVAL;
				return -1;
			} catch (File_system::Permission_denied) {
				errno = EPERM;
				return -1;
			}

			return 0;
		}

		/*
		 * *basep does not get initialized by the libc and is therefore
		 * useless for determining a specific directory index. This
		 * function uses the plugin-internal seek offset instead.
		 */
		ssize_t getdirentries(Libc::File_descriptor *fd, char *buf,
		                      ::size_t nbytes, ::off_t *basep)
		{
			using namespace File_system;

			if (nbytes < sizeof(struct dirent)) {
				PERR("buf too small");
				return -1;
			}

			Directory_entry entry;
			size_t num_bytes = read(fd, &entry, sizeof(entry));

			/* detect end of directory entries */
			if (num_bytes == 0)
				return 0;

			if (num_bytes != sizeof(entry)) {
				PERR("getdirentries retrieved unexpected directory entry size");
				return -1;
			}

			struct dirent *dirent = (struct dirent *)buf;
			Genode::memset(dirent, 0, sizeof(struct dirent));

			switch (entry.type) {
			case Directory_entry::TYPE_DIRECTORY: dirent->d_type = DT_DIR;  break;
			case Directory_entry::TYPE_FILE:      dirent->d_type = DT_REG;  break;
			case Directory_entry::TYPE_SYMLINK:   dirent->d_type = DT_LNK;  break;
			}

			dirent->d_fileno = 1 + (context(fd)->seek_offset() / sizeof(struct dirent));
			dirent->d_reclen = sizeof(struct dirent);

			Genode::strncpy(dirent->d_name, entry.name, sizeof(dirent->d_name));

			dirent->d_namlen = Genode::strlen(dirent->d_name);

			*basep += sizeof(struct dirent);
			return sizeof(struct dirent);
		}

		::off_t lseek(Libc::File_descriptor *fd, ::off_t offset, int whence)
		{
			switch (whence) {

			case SEEK_SET:
				context(fd)->seek_offset(offset);
				return offset;

			case SEEK_CUR:
				context(fd)->advance_seek_offset(offset);
				if (context(fd)->is_appending())
					return _file_size(fd);
				return context(fd)->seek_offset();

			case SEEK_END:
				if (offset != 0) {
					errno = EINVAL;
					return -1;
				}
				context(fd)->infinite_seek_offset();
				return _file_size(fd);

			default:
				errno = EINVAL;
				return -1;
			}
		}

		int mkdir(const char *path, mode_t mode)
		{
			Canonical_path canonical_path(path);

			try {
				File_system::Dir_handle const handle =
					file_system()->dir(canonical_path.base(), true);
				file_system()->close(handle);
				return 0;
			}
			catch (File_system::Permission_denied)   { errno = EPERM; }
			catch (File_system::Node_already_exists) { errno = EEXIST; }
			catch (File_system::Lookup_failed)       { errno = ENOENT; }
			catch (File_system::Name_too_long)       { errno = ENAMETOOLONG; }
			catch (File_system::No_space)            { errno = ENOSPC; }

			return -1;
		}

		Libc::File_descriptor *open(const char *pathname, int flags)
		{
			Canonical_path path(pathname);

			File_system::Mode mode;
			switch (flags & O_ACCMODE) {
			case O_RDONLY: mode = File_system::READ_ONLY;  break;
			case O_WRONLY: mode = File_system::WRITE_ONLY; break;
			case O_RDWR:   mode = File_system::READ_WRITE; break;
			default:       mode = File_system::STAT_ONLY;  break;
			}

			/*
			 * Probe for an existing directory to open
			 */
			try {
				if (verbose)
					PDBG("open dir '%s'", path.base());
				File_system::Dir_handle const handle =
					file_system()->dir(path.base(), false);

				Plugin_context *context = new (Genode::env()->heap())
					Plugin_context(handle);

				return Libc::file_descriptor_allocator()->alloc(this, context);
			} catch (File_system::Lookup_failed) { }

			/*
			 * Determine directory path that contains the node to open
			 */
			Canonical_path dir_path(pathname);
			dir_path.strip_last_element();

			Canonical_path basename(pathname);
			basename.keep_only_last_element();

			try {
				/*
				 * Open directory that contains the file to be opened/created
				 */
				File_system::Dir_handle const dir_handle =
				    file_system()->dir(dir_path.base(), false);

				Node_handle_guard guard(dir_handle);

				File_system::File_handle handle;

				/*
				 * Open or create file
				 */
				bool const create = (flags & O_CREAT) != 0;

				bool opened = false;
				while (!opened) {
					try {
						handle = file_system()->file(dir_handle, basename.base() + 1, mode, create);
						opened = true;
					} catch (File_system::Node_already_exists) {
						if (flags & O_EXCL)
							throw File_system::Node_already_exists();
						/* try to open the existing file */
						try {
							handle = file_system()->file(dir_handle, basename.base() + 1, mode, false);
							opened = true;
						} catch (File_system::Lookup_failed) {
							/* the file got deleted in the meantime */
						}
					}
				}

				Plugin_context *context = new (Genode::env()->heap())
					Plugin_context(handle);

				/*
				 * Prevent returning the file size as current seek offset
				 * for files that are opened read only because this
				 * behaviour is only useful for appending write operations
				 * to files.
				 *
				 * XXX Improve handling of files opened in append mode.
				 */
				if ((flags & O_ACCMODE) == O_RDONLY)
					context->seek_offset(0);

				context->status_flags(flags);

				Libc::File_descriptor *fd = Libc::file_descriptor_allocator()->alloc(this, context);
				if ((flags & O_TRUNC) && (ftruncate(fd, 0) == -1)) {
					Libc::file_descriptor_allocator()->free(fd);
					destroy(Genode::env()->heap(), context);
					return 0;
				}
				return fd;
			}
			catch (File_system::Lookup_failed) {
				PERR("open(%s) lookup failed", pathname); }
			return 0;
		}

		int rename(const char *oldpath, const char *newpath)
		{
			if (verbose)
				PDBG("not implemented");
			return -1;
		}

		ssize_t read(Libc::File_descriptor *fd, void *buf, ::size_t count)
		{
			File_system::Session::Tx::Source &source = *file_system()->tx();

			size_t const max_packet_size = source.bulk_buffer_size() / 2;

			size_t remaining_count = count;

			if (context(fd)->seek_offset() == ~0)
				context(fd)->seek_offset(0);

			while (remaining_count) {

				collect_acknowledgements(source);

				size_t curr_packet_size = Genode::min(remaining_count, max_packet_size);

				/*
				 * XXX handle 'Packet_alloc_failed' exception'
				 */
				File_system::Packet_descriptor
					packet(source.alloc_packet(curr_packet_size),
					       static_cast<File_system::Packet_ref *>(context(fd)),
					       context(fd)->node_handle(),
					       File_system::Packet_descriptor::READ,
					       curr_packet_size,
					       context(fd)->seek_offset());

				/* mark context as having an operation in flight */
				context(fd)->in_flight = true;

				/* pass packet to server side */
				source.submit_packet(packet);

				do {
					packet = source.get_acked_packet();
					static_cast<Plugin_context *>(packet.ref())->in_flight = false;
				} while (context(fd)->in_flight);

				context(fd)->in_flight = false;

				/*
				 * XXX check if acked packet belongs to request,
				 *     needed for thread safety
				 */

				size_t read_num_bytes = Genode::min(packet.length(), curr_packet_size);

				/* copy-out payload into destination buffer */
				memcpy(buf, source.packet_content(packet), read_num_bytes);

				source.release_packet(packet);

				/* prepare next iteration */
				context(fd)->advance_seek_offset(read_num_bytes);
				buf = (void *)((Genode::addr_t)buf + read_num_bytes);
				remaining_count -= read_num_bytes;

				/*
				 * If we received less bytes than requested, we reached the end
				 * of the file.
				 */
				if (read_num_bytes < curr_packet_size)
					break;
			}

			return count - remaining_count;
		}

		ssize_t readlink(const char *path, char *buf, size_t bufsiz)
		{
			if (verbose)
				PDBG("path = %s, bufsiz = %zu", path, bufsiz);

			Canonical_path abs_path(path);
			abs_path.strip_last_element();

			Canonical_path symlink_name(path);
			symlink_name.keep_only_last_element();

			try {
				::File_system::Dir_handle dir_handle = file_system()->dir(abs_path.base(), false);

				::File_system::Symlink_handle symlink_handle =
				    file_system()->symlink(dir_handle, symlink_name.base() + 1, false);

				if (symlink_handle.value == -1) {
					errno = ENOSYS;
					return -1;
				}

				Plugin_context *context = new (Genode::env()->heap())
					Plugin_context(symlink_handle);

				Libc::File_descriptor *fd = Libc::file_descriptor_allocator()->alloc(this, context);

				ssize_t result = read(fd, buf, bufsiz);
				if (verbose)
					PDBG("result = %zd, buf = %s", result, buf);
				close(fd);

				return result;
			} catch (...) { }

			errno = ENOENT;
			return -1;
		}

		int stat(const char *pathname, struct stat *buf)
		{
			if (verbose)
				PDBG("stat %s", pathname);

			Canonical_path path(pathname);

			try {
				File_system::Node_handle const node_handle =
					file_system()->node(path.base());
				Node_handle_guard guard(node_handle);

				obtain_stat_for_node(node_handle, buf);
				return 0;
			}
			catch (File_system::Lookup_failed) {
				PERR("stat(%s): lookup failed", pathname);
				errno = ENOENT;
			}
			return -1;
		}

		int symlink(const char *oldpath, const char *newpath)
		{
			Canonical_path abs_path(newpath);
			abs_path.strip_last_element();

			Canonical_path symlink_name(newpath);
			symlink_name.keep_only_last_element();

			try {
				/*
				 * Open directory that contains the file to be opened/created
				 */
				File_system::Dir_handle const dir_handle =
				    file_system()->dir(abs_path.base(), false);

				Node_handle_guard guard(dir_handle);

				File_system::Symlink_handle symlink_handle =
				    file_system()->symlink(dir_handle, symlink_name.base() + 1, true);

				if (symlink_handle.value == -1) {
					errno = ENOSYS;
					return -1;
				}

				Plugin_context *context = new (Genode::env()->heap())
					Plugin_context(symlink_handle);

				Libc::File_descriptor *fd =
				    Libc::file_descriptor_allocator()->alloc(this, context);

				if (write(fd, oldpath, strlen(oldpath) + 1) == -1) {
					errno = EIO;
					return -1;
				}

				close(fd);

				return 0;
			}
			catch (File_system::Lookup_failed) {
				PERR("symlink(%s) lookup failed", newpath); }

			errno = ENOENT;
			return -1;
		}

		int unlink(const char *path)
		{
			return -1;
		}

		ssize_t write(Libc::File_descriptor *fd, const void *buf, ::size_t count)
		{
			File_system::Session::Tx::Source &source = *file_system()->tx();

			size_t const max_packet_size = source.bulk_buffer_size() / 2;

			size_t remaining_count = count;

			while (remaining_count) {

				collect_acknowledgements(source);

				size_t curr_packet_size = Genode::min(remaining_count, max_packet_size);

				try {
					File_system::Packet_descriptor
						packet(source.alloc_packet(curr_packet_size),
							   static_cast<File_system::Packet_ref *>(context(fd)),
							   context(fd)->node_handle(),
							   File_system::Packet_descriptor::WRITE,
							   curr_packet_size,
							   context(fd)->seek_offset());

					/* mark context as having an operation in flight */
					context(fd)->in_flight = true;

					/* copy-in payload into packet */
					memcpy(source.packet_content(packet), buf, curr_packet_size);

					/* pass packet to server side */
					source.submit_packet(packet);

					/* prepare next iteration */
					context(fd)->advance_seek_offset(curr_packet_size);
					buf = (void *)((Genode::addr_t)buf + curr_packet_size);
					remaining_count -= curr_packet_size;
				} catch (File_system::Session::Tx::Source::Packet_alloc_failed) {
					do {
						wait_for_acknowledgement(source);
					} while (context(fd)->in_flight);
				}
			}

			if (verbose)
				PDBG("write returns %zd", count);
			return count;
		}

		void *mmap(void *addr_in, ::size_t length, int prot, int flags,
		           Libc::File_descriptor *fd, ::off_t offset)
		{
			if (prot != PROT_READ) {
				PERR("mmap for prot=%x not supported", prot);
				errno = EACCES;
				return (void *)-1;
			}

			if (addr_in != 0) {
				PERR("mmap for predefined address not supported");
				errno = EINVAL;
				return (void *)-1;
			}

			void *addr = Libc::mem_alloc()->alloc(length, PAGE_SHIFT);
			if (addr == (void *)-1) {
				errno = ENOMEM;
				return (void *)-1;
			}

			if (::pread(fd->libc_fd, addr, length, offset) < 0) {
				PERR("mmap could not obtain file content");
				::munmap(addr, length);
				errno = EACCES;
				return (void *)-1;
			}

			return addr;
		}

		int munmap(void *addr, ::size_t)
		{
			Libc::mem_alloc()->free(addr);
			return 0;
		}
};


} /* unnamed namespace */


void __attribute__((constructor)) init_libc_fs(void)
{
	PDBG("using the libc_fs plugin");
	static Plugin plugin;
}
