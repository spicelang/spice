; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [17 x i8] c"i is now at: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %i, align 4
  br label %while.head.L3

while.head.L3:                                    ; preds = %while.body.L3, %0
  %1 = load i32, ptr %i, align 4
  %2 = icmp slt i32 %1, 10
  br i1 %2, label %while.body.L3, label %while.exit.L3

while.body.L3:                                    ; preds = %while.head.L3
  %3 = load i32, ptr %i, align 4
  %4 = add nsw i32 %3, 1
  store i32 %4, ptr %i, align 4
  %5 = load i32, ptr %i, align 4
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %5)
  br label %while.head.L3

while.exit.L3:                                    ; preds = %while.head.L3
  %7 = load i32, ptr %result, align 4
  ret i32 %7
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
