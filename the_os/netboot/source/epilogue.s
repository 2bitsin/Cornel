.386
.model tiny

_EPILOGUE segment use16 public para 'EPILOGUE'
  db "YEAHBABYLOADMYOS"
  _loader_start proc near public
  _loader_start endp

_EPILOGUE ends

end
