; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@COUNTER = internal global i32 1

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #0 {
  %1 = load i32, ptr @COUNTER, align 4
  %2 = add nsw i32 %1, 1
  store i32 %2, ptr @COUNTER, align 4
  ret i32 0
}

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"Code Model", i32 2}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 0}
!3 = !{!"spice version dev (https://github.com/spicelang/spice)"}
