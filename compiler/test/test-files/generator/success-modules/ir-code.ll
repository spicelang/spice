; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = call i32 @"forwardToOtherModule()"()
  %1 = alloca i32, align 4
  store i32 %0, i32* %1, align 4
  %2 = load i32, i32* %1, align 4
  %integer = alloca i32, align 4
  store i32 %2, i32* %integer, align 4
  %3 = getelementptr inbounds i32, i32* %integer, i32 0
  %4 = load i32, i32* %3, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i32 %4)
  %6 = load i32, i32* %result, align 4
  ret i32 %6
}

declare i32 @"forwardToOtherModule()"()
