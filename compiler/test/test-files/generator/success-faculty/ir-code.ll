; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define i32 @"faculty(int)"(i32 %0) #0 {
entry:
  %input = alloca i32, align 4
  store i32 %0, i32* %input, align 4
  %result = alloca i32, align 4
  %1 = alloca i32, align 4
  store i32 2, i32* %1, align 4
  %2 = load i32, i32* %input, align 4
  %3 = load i32, i32* %1, align 4
  %4 = icmp slt i32 %2, %3
  %5 = alloca i1, align 1
  store i1 %4, i1* %5, align 1
  %6 = load i1, i1* %5, align 1
  br i1 %6, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %7 = alloca i32, align 4
  store i32 1, i32* %7, align 4
  %8 = load i32, i32* %7, align 4
  ret i32 %8

if.end:                                           ; preds = %entry
  %9 = load i32, i32* %input, align 4
  %10 = load i32, i32* %input, align 4
  %11 = alloca i32, align 4
  store i32 1, i32* %11, align 4
  %12 = load i32, i32* %11, align 4
  %13 = sub i32 %10, %12
  %14 = alloca i32, align 4
  store i32 %13, i32* %14, align 4
  %15 = load i32, i32* %14, align 4
  %16 = call i32 @"faculty(int)"(i32 %15)
  %17 = alloca i32, align 4
  store i32 %16, i32* %17, align 4
  %18 = load i32, i32* %17, align 4
  %19 = mul i32 %9, %18
  %20 = alloca i32, align 4
  store i32 %19, i32* %20, align 4
  %21 = load i32, i32* %20, align 4
  store i32 %21, i32* %result, align 4
  %22 = load i32, i32* %result, align 4
  ret i32 %22
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 10, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  %input = alloca i32, align 4
  store i32 %1, i32* %input, align 4
  %2 = load i32, i32* %input, align 4
  %3 = call i32 @"faculty(int)"(i32 %2)
  %4 = alloca i32, align 4
  store i32 %3, i32* %4, align 4
  %5 = load i32, i32* %4, align 4
  %faculty = alloca i32, align 4
  store i32 %5, i32* %faculty, align 4
  %6 = load i32, i32* %input, align 4
  %7 = load i32, i32* %faculty, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @0, i32 0, i32 0), i32 %6, i32 %7)
  %9 = load i32, i32* %result, align 4
  ret i32 %9
}

attributes #0 = { nounwind }
