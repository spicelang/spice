; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [36 x i8] c"Ackermann of base m=%d and n=%d: %d\00", align 1

; Function Attrs: nounwind
define internal i32 @"ack(int,int)"(i32 %0, i32 %1) #0 {
entry:
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  %result = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i1, align 1
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  store i32 %0, i32* %m, align 4
  store i32 %1, i32* %n, align 4
  store i32 0, i32* %2, align 4
  %18 = load i32, i32* %m, align 4
  %19 = load i32, i32* %2, align 4
  %20 = icmp eq i32 %18, %19
  store i1 %20, i1* %3, align 1
  %21 = load i1, i1* %3, align 1
  br i1 %21, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %22 = load i32, i32* %n, align 4
  store i32 1, i32* %4, align 4
  %23 = load i32, i32* %4, align 4
  %24 = add i32 %22, %23
  store i32 %24, i32* %5, align 4
  %25 = load i32, i32* %5, align 4
  ret i32 %25

if.end:                                           ; preds = %entry
  store i32 0, i32* %6, align 4
  %26 = load i32, i32* %n, align 4
  %27 = load i32, i32* %6, align 4
  %28 = icmp eq i32 %26, %27
  store i1 %28, i1* %7, align 1
  %29 = load i1, i1* %7, align 1
  br i1 %29, label %if.then1, label %if.end2

if.then1:                                         ; preds = %if.end
  %30 = load i32, i32* %m, align 4
  store i32 1, i32* %8, align 4
  %31 = load i32, i32* %8, align 4
  %32 = sub i32 %30, %31
  store i32 %32, i32* %9, align 4
  %33 = load i32, i32* %9, align 4
  store i32 1, i32* %10, align 4
  %34 = load i32, i32* %10, align 4
  %35 = call i32 @"ack(int,int)"(i32 %33, i32 %34)
  store i32 %35, i32* %11, align 4
  %36 = load i32, i32* %11, align 4
  ret i32 %36

if.end2:                                          ; preds = %if.end
  %37 = load i32, i32* %m, align 4
  store i32 1, i32* %12, align 4
  %38 = load i32, i32* %12, align 4
  %39 = sub i32 %37, %38
  store i32 %39, i32* %13, align 4
  %40 = load i32, i32* %13, align 4
  %41 = load i32, i32* %m, align 4
  %42 = load i32, i32* %n, align 4
  store i32 1, i32* %14, align 4
  %43 = load i32, i32* %14, align 4
  %44 = sub i32 %42, %43
  store i32 %44, i32* %15, align 4
  %45 = load i32, i32* %15, align 4
  %46 = call i32 @"ack(int,int)"(i32 %41, i32 %45)
  store i32 %46, i32* %16, align 4
  %47 = load i32, i32* %16, align 4
  %48 = call i32 @"ack(int,int)"(i32 %40, i32 %47)
  store i32 %48, i32* %17, align 4
  %49 = load i32, i32* %17, align 4
  ret i32 %49
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %baseM = alloca i32, align 4
  %0 = alloca i32, align 4
  %baseN = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 3, i32* %0, align 4
  %3 = load i32, i32* %0, align 4
  store i32 %3, i32* %baseM, align 4
  store i32 12, i32* %1, align 4
  %4 = load i32, i32* %1, align 4
  store i32 %4, i32* %baseN, align 4
  %5 = load i32, i32* %baseM, align 4
  %6 = load i32, i32* %baseN, align 4
  %7 = load i32, i32* %baseM, align 4
  %8 = load i32, i32* %baseN, align 4
  %9 = call i32 @"ack(int,int)"(i32 %7, i32 %8)
  store i32 %9, i32* %2, align 4
  %10 = load i32, i32* %2, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @0, i32 0, i32 0), i32 %5, i32 %6, i32 %10)
  %12 = load i32, i32* %result, align 4
  ret i32 %12
}

declare i32 @printf(i8*, ...)

attributes #0 = { nounwind }
