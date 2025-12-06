; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p:32:32-p10:8:8-p20:8:8-i64:64-i128:128-n32:64-S128-ni:1:10:20"
target triple = "wasm32-unknown-unknown"

@printf.str.0 = private unnamed_addr constant [20 x i8] c"Fibonacci of %d: %d\00", align 4

define dso_local noundef i32 @fibo(i32 noundef %0) {
  %result = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, ptr %n, align 4
  %2 = load i32, ptr %n, align 4
  %3 = icmp sle i32 %2, 1
  br i1 %3, label %if.then.L5, label %if.exit.L5

if.then.L5:                                       ; preds = %1
  %4 = load i32, ptr %n, align 4
  ret i32 %4

if.exit.L5:                                       ; preds = %1
  %5 = load i32, ptr %n, align 4
  %6 = sub nsw i32 %5, 1
  %7 = call noundef i32 @fibo(i32 noundef %6)
  %8 = load i32, ptr %n, align 4
  %9 = sub nsw i32 %8, 2
  %10 = call noundef i32 @fibo(i32 noundef %9)
  %11 = add nsw i32 %7, %10
  ret i32 %11
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %fiboBase = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 45, ptr %fiboBase, align 4
  %1 = load i32, ptr %fiboBase, align 4
  %2 = load i32, ptr %fiboBase, align 4
  %3 = call noundef i32 @fibo(i32 noundef %2)
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %1, i32 noundef %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
