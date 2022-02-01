; ModuleID = 'source.spice'
source_filename = "source.spice"

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define i32 @"makePi()"() #0 {
entry:
  ret i32 3
}

define i32 @main() {
entry:
  %0 = tail call i32 @"makePi()"()
  ret i32 0
}

attributes #0 = { nounwind }
