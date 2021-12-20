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
  %lt = icmp slt i32 %3, %4
  %5 = alloca i1, align 1
  store i1 %lt, i1* %5, align 1
  %6 = load i1, i1* %5, align 1
  br i1 %6, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %7 = alloca i32, align 4
  store i32 1, i32* %7, align 4
  %8 = load i32, i32* %7, align 4
  ret i32 %8

if.end:                                           ; preds = %entry
  %9 = getelementptr inbounds i32, i32* %input, i32 0
  %10 = load i32, i32* %9, align 4
  %11 = getelementptr inbounds i32, i32* %input, i32 0
  %12 = load i32, i32* %11, align 4
  %13 = alloca i32, align 4
  store i32 1, i32* %13, align 4
  %14 = load i32, i32* %13, align 4
  %sub = sub i32 %12, %14
  %15 = alloca i32, align 4
  store i32 %sub, i32* %15, align 4
  %16 = load i32, i32* %15, align 4
  %17 = call i32 @"faculty(int)"(i32 %16)
  %18 = alloca i32, align 4
  store i32 %17, i32* %18, align 4
  %19 = getelementptr inbounds i32, i32* %18, i32 0
  %20 = load i32, i32* %19, align 4
  %mul = mul i32 %10, %20
  %21 = alloca i32, align 4
  store i32 %mul, i32* %21, align 4
  %22 = load i32, i32* %21, align 4
  %23 = getelementptr inbounds i32, i32* %result, i32 0
  store i32 %22, i32* %23, align 4
  %24 = load i32, i32* %result, align 4
  ret i32 %24
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
