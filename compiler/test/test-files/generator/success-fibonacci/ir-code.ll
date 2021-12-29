; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define i32 @"fib(int)"(i32 %0) #0 {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %result = alloca i32, align 4
  %1 = getelementptr inbounds i32, i32* %n, i32 0
  %2 = alloca i32, align 4
  store i32 2, i32* %2, align 4
  %3 = load i32, i32* %1, align 4
  %4 = load i32, i32* %2, align 4
  %le = icmp sle i32 %3, %4
  %5 = alloca i1, align 1
  store i1 %le, i1* %5, align 1
  %6 = load i1, i1* %5, align 1
  br i1 %6, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %7 = alloca i32, align 4
  store i32 1, i32* %7, align 4
  %8 = load i32, i32* %7, align 4
  ret i32 %8

if.end:                                           ; preds = %entry
  %9 = getelementptr inbounds i32, i32* %n, i32 0
  %10 = load i32, i32* %9, align 4
  %11 = alloca i32, align 4
  store i32 1, i32* %11, align 4
  %12 = load i32, i32* %11, align 4
  %13 = sub i32 %10, %12
  %14 = alloca i32, align 4
  store i32 %13, i32* %14, align 4
  %15 = load i32, i32* %14, align 4
  %16 = call i32 @"fib(int)"(i32 %15)
  %17 = alloca i32, align 4
  store i32 %16, i32* %17, align 4
  %18 = getelementptr inbounds i32, i32* %17, i32 0
  %19 = load i32, i32* %18, align 4
  %20 = getelementptr inbounds i32, i32* %n, i32 0
  %21 = load i32, i32* %20, align 4
  %22 = alloca i32, align 4
  store i32 2, i32* %22, align 4
  %23 = load i32, i32* %22, align 4
  %24 = sub i32 %21, %23
  %25 = alloca i32, align 4
  store i32 %24, i32* %25, align 4
  %26 = load i32, i32* %25, align 4
  %27 = call i32 @"fib(int)"(i32 %26)
  %28 = alloca i32, align 4
  store i32 %27, i32* %28, align 4
  %29 = getelementptr inbounds i32, i32* %28, i32 0
  %30 = load i32, i32* %29, align 4
  %31 = add i32 %19, %30
  %32 = alloca i32, align 4
  store i32 %31, i32* %32, align 4
  %33 = load i32, i32* %32, align 4
  ret i32 %33
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 46, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  %2 = call i32 @"fib(int)"(i32 %1)
  %3 = alloca i32, align 4
  store i32 %2, i32* %3, align 4
  %4 = getelementptr inbounds i32, i32* %3, i32 0
  %5 = load i32, i32* %4, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i32 %5)
  %7 = load i32, i32* %result, align 4
  ret i32 %7
}

attributes #0 = { nounwind }
