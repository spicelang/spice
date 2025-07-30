; ModuleID = 'lto-module'
source_filename = "lto-module"

@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 4
@str = private unnamed_addr constant [81 x i8] c"Assertion failed: Condition '(functionInModuleB(1, 2) == 3)' evaluated to false.\00", align 1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local i32 @_Z17functionInModuleBii(i32 %0, i32 %1) local_unnamed_addr #0 {
  %3 = add nsw i32 %0, %1
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call i32 @_Z17functionInModuleBii(i32 1, i32 2) #4
  %2 = icmp eq i32 %1, 3
  br i1 %2, label %assert.exit.L4, label %assert.then.L4, !prof !5

assert.then.L4:                                   ; preds = %0
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L4:                                   ; preds = %0
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #2

; Function Attrs: cold nofree noreturn nounwind
declare void @exit(i32) local_unnamed_addr #3

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { cold nofree noreturn nounwind }
attributes #4 = { nounwind }

!llvm.ident = !{!0, !0}
!llvm.module.flags = !{!1, !2, !3, !4}

!0 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"branch_weights", i32 2000, i32 1}
