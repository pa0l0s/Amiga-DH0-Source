/**/
call addlib("rexxreqtools.library",0,-30,0)
call addlib("rexxsupport.library",0,-30,0)
call rtfilerequest(,,,,"rtfi_flags=freqf_multiselect",files)
if files == 1 then
  do i=1 to files.count
    rename(files.i,translate(files.i,xrange('a','z'),xrange('A','Z')))
end