; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [23 x i8] c"Item 0: %d, item 2: %d\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %value0 = alloca i32, align 4
  %1 = alloca [3 x i32], align 4
  %intArray = alloca [3 x i32], align 4
  store i32 0, ptr %result, align 4
  store i32 2, ptr %value0, align 4
  %2 = getelementptr inbounds [3 x i32], ptr %1, i64 0
  %3 = load i32, ptr %value0, align 4
  store i32 %3, ptr %2, align 4
  %4 = getelementptr inbounds i32, ptr %2, i64 1
  store i32 7, ptr %4, align 4
  %5 = getelementptr inbounds i32, ptr %4, i64 1
  store i32 4, ptr %5, align 4
  %6 = load [3 x i32], ptr %1, align 4
  store [3 x i32] %6, ptr %intArray, align 4
  %7 = getelementptr inbounds [3 x i32], ptr %intArray, i64 0, i32 2
  %8 = load i32, ptr %7, align 4
  %9 = mul nsw i32 %8, 11
  store i32 %9, ptr %7, align 4
  %10 = getelementptr inbounds [3 x i32], ptr %intArray, i64 0, i32 0
  store i32 3, ptr %10, align 4
  %11 = getelementptr inbounds [3 x i32], ptr %intArray, i64 0, i32 0
  %12 = load i32, ptr %11, align 4
  %13 = getelementptr inbounds [3 x i32], ptr %intArray, i64 0, i32 2
  %14 = load i32, ptr %13, align 4
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %12, i32 noundef %14)
  %16 = load i32, ptr %result, align 4
  ret i32 %16
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
