  .section ".text.call16_thunk","ax"

  .global call16_thunk_begin
  .global call16_thunk_end

__call16_thunk_begin:
  .incbin CALL16_THUNK_BINARY
__call16_thunk_end:

// ---------------------------------------------------------------------------

  .global call16_thunk_begin
call16_thunk_begin:
  .long __call16_thunk_begin

  .global call16_thunk_end
call16_thunk_end:
  .long __call16_thunk_end

  .global call16_thunk_target
call16_thunk_target:
  .long CALL16_THUNK_ORG

