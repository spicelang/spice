; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %loopCounterOuter = alloca double, align 8
  %0 = alloca double, align 8
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  %loopCounterInner = alloca i16, align 2
  %3 = alloca i16, align 2
  %4 = alloca i1, align 1
  %5 = alloca i16, align 2
  store i32 0, i32* %result, align 4
  store double 0.000000e+00, double* %0, align 8
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %6 = load double, double* %0, align 8
  %7 = fcmp olt double %6, 1.000000e+01
  store i1 %7, i1* %1, align 1
  %8 = load i1, i1* %1, align 1
  br i1 %8, label %while, label %while.end3

while:                                            ; preds = %while.cond
  %9 = load double, double* %0, align 8
  %10 = fcmp olt double %9, 4.000000e+00
  store i1 %10, i1* %2, align 1
  %11 = load i1, i1* %2, align 1
  br i1 %11, label %if.then, label %if.end

if.then:                                          ; preds = %while
  store i16 10, i16* %3, align 2
  br label %while.cond1

while.cond1:                                      ; preds = %if.then
  %12 = load i16, i16* %3, align 2
  %13 = sext i16 %12 to i32
  %14 = icmp sgt i32 %13, 0
  store i1 %14, i1* %4, align 1
  %15 = load i1, i1* %4, align 1
  br i1 %15, label %while2, label %while.end

while2:                                           ; preds = %while.cond1
  %16 = load double, double* %0, align 8
  %17 = load i16, i16* %3, align 2
  %18 = sext i16 %17 to i32
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), double %16, i32 %18)
  %20 = load i16, i16* %3, align 2
  %21 = sub i16 %20, 1
  store i16 %21, i16* %3, align 2
  store i16 %20, i16* %5, align 2
  br label %while.end3

while.end:                                        ; preds = %while.cond1
  br label %if.end

if.end:                                           ; preds = %while.end, %while
  %22 = load double, double* %0, align 8
  %23 = fadd double %22, 1.500000e-01
  store double %23, double* %0, align 8
  br label %while.cond

while.end3:                                       ; preds = %while2, %while.cond
  %24 = load i32, i32* %result, align 4
  ret i32 %24
}

declare i32 @printf(i8*, ...)
