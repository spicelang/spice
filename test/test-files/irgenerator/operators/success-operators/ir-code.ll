; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [60 x i8] c"Assertion failed: Condition 'val == 9' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %val = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 1, ptr %val, align 4
  store i32 2, ptr %1, align 4
  store i32 0, ptr %2, align 4
  %3 = load i32, ptr %val, align 4
  %4 = add nsw i32 %3, 8
  store i32 %4, ptr %val, align 4
  %5 = load i32, ptr %val, align 4
  %6 = icmp eq i32 %5, 9
  br i1 %6, label %assert.exit.L4, label %assert.then.L4, !prof !5

assert.then.L4:                                   ; preds = %0
  %7 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L4:                                   ; preds = %0
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 2000, i32 1}
