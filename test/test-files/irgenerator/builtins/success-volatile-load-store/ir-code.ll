; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #0 {
  %i = alloca i32, align 4
  %iPtr = alloca ptr, align 8
  %loaded = alloca i32, align 4
  %d = alloca double, align 8
  %color = alloca i32, align 4
  %iPtrRef = alloca ptr, align 8
  %iPtrPtr = alloca ptr, align 8
  store i32 1, ptr %i, align 4
  store ptr %i, ptr %iPtr, align 8
  %1 = load ptr, ptr %iPtr, align 8
  store volatile i32 42, ptr %1, align 4
  %2 = load ptr, ptr %iPtr, align 8
  %3 = load volatile i32, ptr %2, align 4
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3)
  store volatile i32 1, ptr %i, align 4
  store volatile i32 2, ptr %i, align 4
  %5 = load volatile i32, ptr %i, align 4
  store i32 %5, ptr %loaded, align 4
  %6 = load i32, ptr %loaded, align 4
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %6)
  store double 1.500000e+00, ptr %d, align 8
  store volatile double 2.500000e+00, ptr %d, align 8
  %8 = load volatile double, ptr %d, align 8
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, double noundef %8)
  store i32 0, ptr %color, align 4
  store volatile i32 1, ptr %color, align 4
  %10 = load volatile i32, ptr %color, align 4
  %11 = icmp eq i32 %10, 1
  %12 = zext i1 %11 to i32
  %13 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %12)
  store ptr %iPtr, ptr %iPtrRef, align 8
  store ptr %iPtr, ptr %iPtrPtr, align 8
  %14 = load ptr, ptr %iPtrRef, align 8
  %15 = load ptr, ptr %14, align 8
  store volatile i32 7, ptr %15, align 4
  %16 = load ptr, ptr %iPtrPtr, align 8
  %17 = load volatile ptr, ptr %16, align 8
  %18 = load i32, ptr %17, align 4
  %19 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %18)
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
