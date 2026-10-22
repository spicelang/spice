; ModuleID = 'source.spice'
source_filename = "source.spice"

@INFERRED_INT = internal global i32 5
@INFERRED_DOUBLE = internal global double 2.500000e+00
@INFERRED_STRING = internal global ptr @anon.string.0
@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 4
@INFERRED_CONST_BOOL = internal constant i1 true
@printf.str.0 = private unnamed_addr constant [24 x i8] c"Values: %d, %f, %s, %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [13 x i8] c"Exported: %d\00", align 4
@EXPORTED_INFERRED_INT = external global i32

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #0 {
  %1 = load i32, ptr @INFERRED_INT, align 4
  %2 = add nsw i32 %1, 1
  store i32 %2, ptr @INFERRED_INT, align 4
  %3 = load i32, ptr @INFERRED_INT, align 4
  %4 = load double, ptr @INFERRED_DOUBLE, align 8
  %5 = load ptr, ptr @INFERRED_STRING, align 8
  %6 = load i1, ptr @INFERRED_CONST_BOOL, align 1
  %7 = zext i1 %6 to i32
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3, double noundef %4, ptr noundef %5, i32 noundef %7)
  %9 = load i32, ptr @EXPORTED_INFERRED_INT, align 4
  %10 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %9)
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
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
