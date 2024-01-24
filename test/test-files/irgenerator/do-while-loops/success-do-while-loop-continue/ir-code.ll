; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [22 x i8] c"Outer: %f, inner: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %loopCounterOuter = alloca double, align 8
  %loopCounterInner = alloca i16, align 2
  store i32 0, ptr %result, align 4
  store double 0.000000e+00, ptr %loopCounterOuter, align 8
  br label %dowhile.body.L3

dowhile.body.L3:                                  ; preds = %dowhile.foot.L3, %0
  %1 = load double, ptr %loopCounterOuter, align 8
  %2 = fadd double %1, 1.500000e-01
  store double %2, ptr %loopCounterOuter, align 8
  %3 = load double, ptr %loopCounterOuter, align 8
  %4 = fcmp olt double %3, 4.000000e+00
  br i1 %4, label %if.then.L5, label %if.exit.L5

if.then.L5:                                       ; preds = %dowhile.body.L3
  store i16 10, ptr %loopCounterInner, align 2
  br label %dowhile.body.L7

dowhile.body.L7:                                  ; preds = %dowhile.foot.L7, %if.then.L5
  %5 = load double, ptr %loopCounterOuter, align 8
  %6 = load i16, ptr %loopCounterInner, align 2
  %7 = sext i16 %6 to i32
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %5, i32 %7)
  %9 = load i16, ptr %loopCounterInner, align 2
  %10 = sub nsw i16 %9, 1
  store i16 %10, ptr %loopCounterInner, align 2
  %11 = load i16, ptr %loopCounterInner, align 2
  %12 = sext i16 %11 to i32
  %13 = icmp eq i32 %12, 5
  br i1 %13, label %if.then.L10, label %if.exit.L10

if.then.L10:                                      ; preds = %dowhile.body.L7
  br label %dowhile.foot.L3

if.exit.L10:                                      ; preds = %dowhile.body.L7
  br label %dowhile.foot.L7

dowhile.foot.L7:                                  ; preds = %if.exit.L10
  %14 = load i16, ptr %loopCounterInner, align 2
  %15 = sext i16 %14 to i32
  %16 = icmp sgt i32 %15, 0
  br i1 %16, label %dowhile.body.L7, label %dowhile.exit.L7

dowhile.exit.L7:                                  ; preds = %dowhile.foot.L7
  br label %if.exit.L5

if.exit.L5:                                       ; preds = %dowhile.exit.L7, %dowhile.body.L3
  br label %dowhile.foot.L3

dowhile.foot.L3:                                  ; preds = %if.exit.L5, %if.then.L10
  %17 = load double, ptr %loopCounterOuter, align 8
  %18 = fcmp olt double %17, 1.000000e+01
  br i1 %18, label %dowhile.body.L3, label %dowhile.exit.L3

dowhile.exit.L3:                                  ; preds = %dowhile.foot.L3
  %19 = load i32, ptr %result, align 4
  ret i32 %19
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
