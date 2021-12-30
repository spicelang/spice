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
  %5 = icmp sle i32 %3, %4
  %6 = alloca i1, align 1
  store i1 %5, i1* %6, align 1
  %7 = load i1, i1* %6, align 1
  br i1 %7, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %8 = alloca i32, align 4
  store i32 1, i32* %8, align 4
  %9 = load i32, i32* %8, align 4
  ret i32 %9

if.end:                                           ; preds = %entry
  %10 = getelementptr inbounds i32, i32* %n, i32 0
  %11 = load i32, i32* %10, align 4
  %12 = alloca i32, align 4
  store i32 1, i32* %12, align 4
  %13 = load i32, i32* %12, align 4
  %14 = sub i32 %11, %13
  %15 = alloca i32, align 4
  store i32 %14, i32* %15, align 4
  %16 = load i32, i32* %15, align 4
  %17 = call i32 @"fib(int)"(i32 %16)
  %18 = alloca i32, align 4
  store i32 %17, i32* %18, align 4
  %19 = getelementptr inbounds i32, i32* %18, i32 0
  %20 = load i32, i32* %19, align 4
  %21 = getelementptr inbounds i32, i32* %n, i32 0
  %22 = load i32, i32* %21, align 4
  %23 = alloca i32, align 4
  store i32 2, i32* %23, align 4
  %24 = load i32, i32* %23, align 4
  %25 = sub i32 %22, %24
  %26 = alloca i32, align 4
  store i32 %25, i32* %26, align 4
  %27 = load i32, i32* %26, align 4
  %28 = call i32 @"fib(int)"(i32 %27)
  %29 = alloca i32, align 4
  store i32 %28, i32* %29, align 4
  %30 = getelementptr inbounds i32, i32* %29, i32 0
  %31 = load i32, i32* %30, align 4
  %32 = add i32 %20, %31
  %33 = alloca i32, align 4
  store i32 %32, i32* %33, align 4
  %34 = load i32, i32* %33, align 4
  ret i32 %34
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
