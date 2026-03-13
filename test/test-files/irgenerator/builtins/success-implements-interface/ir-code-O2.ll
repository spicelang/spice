; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.1 = private unnamed_addr constant [41 x i8] c"Implements interface (ITest, Test1): %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [41 x i8] c"Implements interface (ITest, Test2): %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [40 x i8] c"Implements interface (ITest, bool): %d\0A\00", align 4

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() local_unnamed_addr #1 {
  %1 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 noundef 1)
  %2 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, i32 noundef 0)
  %3 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 noundef 0)
  ret i32 0
}

attributes #0 = { nofree nounwind }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
