; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [7 x i8] c"2: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [7 x i8] c"3: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [7 x i8] c"4: %d\0A\00", align 4
@anon.array.0 = private unnamed_addr constant [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@printf.str.4 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 4

define private void @_Z8testProcPPPA4_i(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %nums = alloca ptr, align 8
  %nums1 = alloca ptr, align 8
  %nums2 = alloca [4 x i32], align 4
  store ptr %0, ptr %nums, align 8
  %2 = load ptr, ptr %nums, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = load ptr, ptr %3, align 8
  store ptr %4, ptr %nums1, align 8
  %5 = load ptr, ptr %nums1, align 8
  %6 = load [4 x i32], ptr %5, align 4
  store [4 x i32] %6, ptr %nums2, align 4
  %7 = getelementptr inbounds [4 x i32], ptr %nums2, i64 0, i32 2
  store i32 10, ptr %7, align 4
  %8 = getelementptr inbounds [4 x i32], ptr %nums2, i64 0, i32 0
  %9 = load i32, ptr %8, align 4
  %10 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %9)
  %11 = getelementptr inbounds [4 x i32], ptr %nums2, i64 0, i32 1
  %12 = load i32, ptr %11, align 4
  %13 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %12)
  %14 = getelementptr inbounds [4 x i32], ptr %nums2, i64 0, i32 2
  %15 = load i32, ptr %14, align 4
  %16 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %15)
  %17 = getelementptr inbounds [4 x i32], ptr %nums2, i64 0, i32 3
  %18 = load i32, ptr %17, align 4
  %19 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %18)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  %intArray = alloca [4 x i32], align 4
  %1 = alloca ptr, align 8
  %2 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store [4 x i32] [i32 1, i32 2, i32 3, i32 4], ptr %intArray, align 4
  %3 = getelementptr inbounds [4 x i32], ptr %intArray, i64 0, i32 1
  %4 = load i32, ptr %3, align 4
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %4)
  store ptr %intArray, ptr %1, align 8
  store ptr %1, ptr %2, align 8
  call void @_Z8testProcPPPA4_i(ptr noundef align 8 dereferenceable(8) %2)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
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
