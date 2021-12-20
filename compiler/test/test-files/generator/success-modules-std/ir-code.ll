; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i1, align 1
  store i1 true, i1* %0, align 1
  %1 = load i1, i1* %0, align 1
  %2 = call i32 @"toInt(bool)"(i1 %1)
  %3 = alloca i32, align 4
  store i32 %2, i32* %3, align 4
  %4 = getelementptr inbounds i32, i32* %3, i32 0
  %5 = load i32, i32* %4, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i32 %5)
  %7 = load i32, i32* %result, align 4
  ret i32 %7
}

declare i32 @"toInt(bool)"(i1)
