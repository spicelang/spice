; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0))
  %1 = load i32, i32* %result, align 4
  ret i32 %1
}
