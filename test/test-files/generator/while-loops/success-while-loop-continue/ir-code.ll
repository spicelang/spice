; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %loopCounterOuter = alloca double, align 8
  %0 = alloca i1, align 1
  %1 = alloca i1, align 1
  %loopCounterInner = alloca i16, align 2
  %2 = alloca i1, align 1
  %3 = alloca i16, align 2
  %4 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store double 0.000000e+00, double* %loopCounterOuter, align 8
  br label %while.cond

while.cond:                                       ; preds = %if.end4, %if.then3, %entry
  %5 = load double, double* %loopCounterOuter, align 8
  %6 = fcmp olt double %5, 1.000000e+01
  store i1 %6, i1* %0, align 1
  %7 = load i1, i1* %0, align 1
  br i1 %7, label %while, label %while.end5

while:                                            ; preds = %while.cond
  %8 = load double, double* %loopCounterOuter, align 8
  %9 = fadd double %8, 1.500000e-01
  store double %9, double* %loopCounterOuter, align 8
  %10 = load double, double* %loopCounterOuter, align 8
  %11 = fcmp olt double %10, 4.000000e+00
  store i1 %11, i1* %1, align 1
  %12 = load i1, i1* %1, align 1
  br i1 %12, label %if.then, label %if.end4

if.then:                                          ; preds = %while
  store i16 10, i16* %loopCounterInner, align 2
  br label %while.cond1

while.cond1:                                      ; preds = %if.end, %if.then
  %13 = load i16, i16* %loopCounterInner, align 2
  %14 = sext i16 %13 to i32
  %15 = icmp sgt i32 %14, 0
  store i1 %15, i1* %2, align 1
  %16 = load i1, i1* %2, align 1
  br i1 %16, label %while2, label %while.end

while2:                                           ; preds = %while.cond1
  %17 = load double, double* %loopCounterOuter, align 8
  %18 = load i16, i16* %loopCounterInner, align 2
  %19 = sext i16 %18 to i32
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), double %17, i32 %19)
  %21 = load i16, i16* %loopCounterInner, align 2
  %22 = sub i16 %21, 1
  store i16 %22, i16* %loopCounterInner, align 2
  store i16 %21, i16* %3, align 2
  %23 = load i16, i16* %loopCounterInner, align 2
  %24 = sext i16 %23 to i32
  %25 = icmp eq i32 %24, 5
  store i1 %25, i1* %4, align 1
  %26 = load i1, i1* %4, align 1
  br i1 %26, label %if.then3, label %if.end

if.then3:                                         ; preds = %while2
  br label %while.cond

if.end:                                           ; preds = %while2
  br label %while.cond1

while.end:                                        ; preds = %while.cond1
  br label %if.end4

if.end4:                                          ; preds = %while.end, %while
  br label %while.cond

while.end5:                                       ; preds = %while.cond
  %27 = load i32, i32* %result, align 4
  ret i32 %27
}

declare i32 @printf(i8*, ...)
