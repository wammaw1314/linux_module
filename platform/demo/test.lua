local ffi = require("ffi")
local C = ffi.C
ffi.cdef[[
int open(const char *path, int flags, int mode);
int close(int fd);
int ioctl(int fd, int cmd,unsigned long arg);
]]

local magic=0x20000000 + string.byte('J')*0x100

local chrfd = C.open("/dev/chrdev", 2, 0)
local cmd=magic+1
print(C.ioctl(chrfd, cmd, 0))
C.close(chrfd)