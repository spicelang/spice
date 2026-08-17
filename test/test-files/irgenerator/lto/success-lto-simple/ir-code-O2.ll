; ModuleID = 'lto-module'
source_filename = "lto-module"

@str.1 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1, !guid !0

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @_Z17functionInModuleBii(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 !guid !6 {
  %3 = add nsw i32 %1, %0
  ret i32 %3
}

; Function Attrs: mustprogress nofree noinline norecurse nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #1 !guid !7 {
assert.exit.L6:
  %puts1 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #2

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable }
attributes #1 = { mustprogress nofree noinline norecurse nounwind uwtable }
attributes #2 = { nofree nounwind }

!llvm.ident = !{!1}
!llvm.module.flags = !{!2, !3, !4, !5}

!0 = !{i64 7093348567730294648}
!1 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"PIE Level", i32 2}
!4 = !{i32 7, !"uwtable", i32 2}
!5 = !{i32 7, !"frame-pointer", i32 2}
!6 = !{i64 1143632153537408826}
!7 = !{i64 -2624081020897602054}
