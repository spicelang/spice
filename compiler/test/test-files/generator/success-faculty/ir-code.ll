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
  %1 = getelementptr inbounds i32, i32* %input, i32 0
  %2 = alloca i32, align 4
  store i32 2, i32* %2, align 4
  %3 = load i32, i32* %1, align 4
  %4 = load i32, i32* %2, align 4
  %5 = icmp slt i32 %3, %4
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
  %10 = getelementptr inbounds i32, i32* %input, i32 0
  %11 = load i32, i32* %10, align 4
  %12 = getelementptr inbounds i32, i32* %input, i32 0
  %13 = load i32, i32* %12, align 4
  %14 = alloca i32, align 4
  store i32 1, i32* %14, align 4
  %15 = load i32, i32* %14, align 4
  %16 = sub i32 %13, %15
  %17 = alloca i32, align 4
  store i32 %16, i32* %17, align 4
  %18 = load i32, i32* %17, align 4
  %19 = call i32 @"faculty(int)"(i32 %18)
  %20 = alloca i32, align 4
  store i32 %19, i32* %20, align 4
  %21 = getelementptr inbounds i32, i32* %20, i32 0
  %22 = load i32, i32* %21, align 4
  %23 = mul i32 %11, %22
  %24 = alloca i32, align 4
  store i32 %23, i32* %24, align 4
  %25 = load i32, i32* %24, align 4
  %26 = getelementptr inbounds i32, i32* %result, i32 0
  store i32 %25, i32* %26, align 4
  %27 = load i32, i32* %result, align 4
  ret i32 %27
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
  %2 = getelementptr inbounds i32, i32* %input, i32 0
  %3 = load i32, i32* %2, align 4
  %4 = call i32 @"faculty(int)"(i32 %3)
  %5 = alloca i32, align 4
  store i32 %4, i32* %5, align 4
  %6 = getelementptr inbounds i32, i32* %5, i32 0
  %7 = load i32, i32* %6, align 4
  %faculty = alloca i32, align 4
  store i32 %7, i32* %faculty, align 4
  %8 = getelementptr inbounds i32, i32* %input, i32 0
  %9 = load i32, i32* %8, align 4
  %10 = getelementptr inbounds i32, i32* %faculty, i32 0
  %11 = load i32, i32* %10, align 4
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @0, i32 0, i32 0), i32 %9, i32 %11)
  %13 = load i32, i32* %result, align 4
  ret i32 %13
}

attributes #0 = { nounwind }
