; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [11 x i8] c"Value: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  store i32 1, i32* %1, align 4
  %6 = load i32, i32* %1, align 4
  %7 = add i32 %6, 1
  store i32 %7, i32* %1, align 4
  %8 = load i32, i32* %1, align 4
  %9 = sub i32 %8, 1
  store i32 %9, i32* %1, align 4
  %10 = load i32, i32* %1, align 4
  store i32 2, i32* %2, align 4
  %11 = load i32, i32* %2, align 4
  %12 = mul i32 %10, %11
  store i32 %12, i32* %3, align 4
  store i32 2, i32* %4, align 4
  %13 = load i32, i32* %3, align 4
  %14 = load i32, i32* %4, align 4
  %15 = shl i32 %13, %14
  store i32 %15, i32* %5, align 4
  %16 = load i32, i32* %5, align 4
  %17 = load i32, i32* %0, align 4
  %18 = add i32 %17, %16
  store i32 %18, i32* %0, align 4
  %19 = load i32, i32* %0, align 4
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i32 %19)
  %21 = load i32, i32* %result, align 4
  ret i32 %21
}
