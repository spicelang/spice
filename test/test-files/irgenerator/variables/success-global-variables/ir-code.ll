; ModuleID = 'source.spice'
source_filename = "source.spice"

@TEST1 = private global i32 10
@TEST2 = private global ptr @anon.string.0
@anon.string.0 = private unnamed_addr constant [12 x i8] c"test string\00", align 4
@TEST3 = private global double 5.830000e+00
@TEST4 = private global i1 false
@anon.string.1 = private unnamed_addr constant [5 x i8] c"test\00", align 4
@printf.str.0 = private unnamed_addr constant [32 x i8] c"Variable values: %d, %s, %f, %u\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 11, ptr @TEST1, align 4
  store ptr @anon.string.1, ptr @TEST2, align 8
  store double 5.840000e+00, ptr @TEST3, align 8
  store i1 true, ptr @TEST4, align 1
  %1 = load i32, ptr @TEST1, align 4
  %2 = load ptr, ptr @TEST2, align 8
  %3 = load double, ptr @TEST3, align 8
  %4 = load i1, ptr @TEST4, align 1
  %5 = zext i1 %4 to i32
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %1, ptr noundef %2, double noundef %3, i32 noundef %5)
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
