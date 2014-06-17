pro view,n
  nx = 100/n
  ny = 100
  m = lonarr(ny,nx)
  while (1) do begin
     for j=0,n-1 do begin
        filename=strcompress("out"+string(j),/remove_all)
        openr, 1, filename, /rawio
        readu,1,m
        close,1
;        loadct,j*4
        tvscl,congrid(transpose(m),500/n,500),j*500/n,0
     endfor
     wait,0.1
  endwhile
return
end
