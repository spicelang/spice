; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [10 x i8] c"Value: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 1, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  %val = alloca i32, align 4
  store i32 %1, i32* %val, align 4
  %2 = alloca i32, align 4
  store i32 1, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %post_pp = add i32 %3, 1
  store i32 %post_pp, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  %post_mm = sub i32 %4, 1
  store i32 %post_mm, i32* %2, align 4
  %5 = load i32, i32* %2, align 4
  %6 = alloca i32, align 4
  store i32 2, i32* %6, align 4
  %7 = load i32, i32* %6, align 4
  %8 = mul i32 %5, %7
  %9 = alloca i32, align 4
  store i32 %8, i32* %9, align 4
  %10 = alloca i32, align 4
  store i32 2, i32* %10, align 4
  %11 = load i32, i32* %9, align 4
  %12 = load i32, i32* %10, align 4
  %shl = shl i32 %11, %12
  %13 = alloca i32, align 4
  store i32 %shl, i32* %13, align 4
  %14 = load i32, i32* %13, align 4
  %15 = getelementptr inbounds i32, i32* %val, i32 0
  %16 = load i32, i32* %15, align 4
  %add = add i32 %16, %14
  store i32 %add, i32* %15, align 4
  %17 = getelementptr inbounds i32, i32* %val, i32 0
  %18 = load i32, i32* %17, align 4
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %18)
  %20 = load i32, i32* %result, align 4
  ret i32 %20
}
