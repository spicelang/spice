; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 't == 3' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 't == 6' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

define private void @_Z3fooIRiEvRi(ptr %0) {
  %t = alloca ptr, align 8
  store ptr %0, ptr %t, align 8
  %2 = load ptr, ptr %t, align 8
  %3 = load i32, ptr %2, align 4
  %4 = add nsw i32 %3, 2
  store i32 %4, ptr %2, align 4
  ret void
}

define private void @_Z3barRi(ptr %0) {
  %t = alloca ptr, align 8
  store ptr %0, ptr %t, align 8
  %2 = load ptr, ptr %t, align 8
  %3 = load i32, ptr %2, align 4
  %4 = add nsw i32 %3, 3
  store i32 %4, ptr %2, align 4
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %t = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 1, ptr %t, align 4
  call void @_Z3fooIRiEvRi(ptr %t)
  %1 = load i32, ptr %t, align 4
  %2 = icmp eq i32 %1, 3
  br i1 %2, label %assert.exit.L13, label %assert.then.L13, !prof !5

assert.then.L13:                                  ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %0
  call void @_Z3barRi(ptr %t)
  %4 = load i32, ptr %t, align 4
  %5 = icmp eq i32 %4, 6
  br i1 %5, label %assert.exit.L15, label %assert.then.L15, !prof !5

assert.then.L15:                                  ; preds = %assert.exit.L13
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L13
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
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
