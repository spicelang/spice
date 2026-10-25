; ModuleID = 'source.spice'
source_filename = "source.spice"

@COUNTER = internal global i32 1
@printf.str.0 = private unnamed_addr constant [13 x i8] c"Counter: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #0 {
  %1 = load i32, ptr @COUNTER, align 4
  %2 = add nsw i32 %1, 1
  store i32 %2, ptr @COUNTER, align 4
  %3 = load i32, ptr @COUNTER, align 4
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 1, !"Code Model", i32 4}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 0}
!5 = !{!"spice version dev (https://github.com/spicelang/spice)"}
