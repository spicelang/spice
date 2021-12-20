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
  %sub = sub i32 %10, %12
  %13 = alloca i32, align 4
  store i32 %sub, i32* %13, align 4
  %14 = load i32, i32* %13, align 4
  %15 = call i32 @"fib(int)"(i32 %14)
  %16 = alloca i32, align 4
  store i32 %15, i32* %16, align 4
  %17 = getelementptr inbounds i32, i32* %16, i32 0
  %18 = load i32, i32* %17, align 4
  %19 = getelementptr inbounds i32, i32* %n, i32 0
  %20 = load i32, i32* %19, align 4
  %21 = alloca i32, align 4
  store i32 2, i32* %21, align 4
  %22 = load i32, i32* %21, align 4
  %sub1 = sub i32 %20, %22
  %23 = alloca i32, align 4
  store i32 %sub1, i32* %23, align 4
  %24 = load i32, i32* %23, align 4
  %25 = call i32 @"fib(int)"(i32 %24)
  %26 = alloca i32, align 4
  store i32 %25, i32* %26, align 4
  %27 = getelementptr inbounds i32, i32* %26, i32 0
  %28 = load i32, i32* %27, align 4
  %add = add i32 %18, %28
  %29 = alloca i32, align 4
  store i32 %add, i32* %29, align 4
  %30 = load i32, i32* %29, align 4
  ret i32 %30
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
