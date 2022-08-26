.section ".text.bootstrap","ax"
.global bootstrap_entry
bootstrap_entry:
.incbin BOOTSTRAP_BINARY
