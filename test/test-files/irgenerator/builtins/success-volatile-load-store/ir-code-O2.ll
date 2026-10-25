; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4

; Function Attrs: mustprogress nofree noinline norecurse nounwind uwtable
define noundef i32 @main() local_unnamed_addr #0 {
  %i = alloca i32, align 4
  %iPtr = alloca ptr, align 8
  %d = alloca double, align 8
  %color = alloca i32, align 4
  %c = alloca i32, align 4
  store ptr %i, ptr %iPtr, align 8
  store volatile i32 42, ptr %i, align 4
  %1 = load volatile i32, ptr %i, align 4
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i32 noundef %1)
  store volatile i32 1, ptr %i, align 4
  store volatile i32 2, ptr %i, align 4
  %3 = load volatile i32, ptr %i, align 4
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i32 noundef %3)
  store volatile double 2.500000e+00, ptr %d, align 8
  %d.0.d.0.d.0.d.0. = load volatile double, ptr %d, align 8
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, double noundef %d.0.d.0.d.0.d.0.)
  store volatile i32 1, ptr %color, align 4
  %color.0.color.0.color.0.color.0. = load volatile i32, ptr %color, align 4
  %6 = icmp eq i32 %color.0.color.0.color.0.color.0., 1
  %7 = zext i1 %6 to i32
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i32 noundef %7)
  store volatile i32 7, ptr %i, align 4
  %iPtr.0.iPtr.0.iPtr.0.iPtr.0.3 = load volatile ptr, ptr %iPtr, align 8
  %9 = load i32, ptr %iPtr.0.iPtr.0.iPtr.0.iPtr.0.3, align 4
  %10 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i32 noundef %9)
  store i32 3, ptr %c, align 4
  %c.0.c.0.c.0.c.0. = load volatile i32, ptr %c, align 4
  %11 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i32 noundef %c.0.c.0.c.0.c.0.)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress nofree noinline norecurse nounwind uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
