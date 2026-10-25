; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Bits: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z9countBitsm(i64 noundef %0) #0 {
  %value = alloca i64, align 8
  %count = alloca i32, align 4
  store i64 %0, ptr %value, align 8
  store i32 0, ptr %count, align 4
  br label %while.head.L7

while.head.L7:                                    ; preds = %while.body.L7, %1
  %2 = load i64, ptr %value, align 8
  %3 = icmp ne i64 %2, 0
  br i1 %3, label %while.body.L7, label %while.exit.L7

while.body.L7:                                    ; preds = %while.head.L7
  %4 = load i64, ptr %value, align 8
  %5 = sub i64 %4, 1
  %6 = load i64, ptr %value, align 8
  %7 = and i64 %6, %5
  store i64 %7, ptr %value, align 8
  %8 = load i32, ptr %count, align 4
  %9 = add nsw i32 %8, 1
  store i32 %9, ptr %count, align 4
  br label %while.head.L7

while.exit.L7:                                    ; preds = %while.head.L7
  %10 = load i32, ptr %count, align 4
  ret i32 %10
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #1 {
  %1 = call noundef i32 @_Z9countBitsm(i64 noundef 255)
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
