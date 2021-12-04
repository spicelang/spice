; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [11 x i8] c"Result: %f\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i32)

declare void @free(i8*)

declare i8* @memcpy(i8*, i8*, i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 5, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  %2 = call double @"toDouble(int)"(i32 %1)
  %3 = alloca double, align 8
  store double %2, double* %3, align 8
  %4 = load double, double* %3, align 8
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), double %4)
  %6 = load i32, i32* %result, align 4
  ret i32 %6
}

declare double @"toDouble(int)"(i32)
