ENTRY(_start)

SECTIONS {
  . = 0x1169;
  .text : {
    *(.text)
  }
  .data : {
    *(.data)
  }
}

