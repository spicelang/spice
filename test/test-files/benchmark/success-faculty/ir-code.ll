; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 4

define private i32 @_Z7facultyi(i32 %0) {
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  store i32 %0, ptr %input, align 4
  %2 = load i32, ptr %input, align 4
  %3 = icmp slt i32 %2, 2
  br i1 %3, label %if.then.L2, label %if.exit.L2

if.then.L2:                                       ; preds = %1
  ret i32 1

if.exit.L2:                                       ; preds = %1
  %4 = load i32, ptr %input, align 4
  %5 = sub nsw i32 %4, 1
  %6 = call i32 @_Z7facultyi(i32 %5)
  %7 = load i32, ptr %input, align 4
  %8 = mul nsw i32 %7, %6
  store i32 %8, ptr %result, align 4
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %faculty = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 10, ptr %input, align 4
  %1 = load i32, ptr %input, align 4
  %2 = call i32 @_Z7facultyi(i32 %1)
  store i32 %2, ptr %faculty, align 4
  %3 = load i32, ptr %input, align 4
  %4 = load i32, ptr %faculty, align 4
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3, i32 noundef %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
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
