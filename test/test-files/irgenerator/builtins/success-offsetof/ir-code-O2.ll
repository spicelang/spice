; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [23 x i8] c"Offset of _fieldA: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [23 x i8] c"Offset of _fieldB: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [23 x i8] c"Offset of _fieldC: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [23 x i8] c"Offset of _fieldD: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [23 x i8] c"Offset of _fieldE: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [25 x i8] c"Offset of _fieldE.a: %d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [25 x i8] c"Offset of _fieldE.b: %d\0A\00", align 4
@printf.str.7 = private unnamed_addr constant [29 x i8] c"Const offset of _fieldD: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  %1 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 noundef 0)
  %2 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i64 noundef 4)
  %3 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, i64 noundef 8)
  %4 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i64 noundef 16)
  %5 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4, i64 noundef 24)
  %6 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i64 noundef 24)
  %7 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.6, i64 noundef 32)
  %8 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7, i64 noundef 16)
  ret i32 0
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
