; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 't == 3' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 't == 6' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z3fooIRiEvRi(ptr noundef %0) #0 {
  %t = alloca ptr, align 8
  store ptr %0, ptr %t, align 8
  %2 = load ptr, ptr %t, align 8
  %3 = load i32, ptr %2, align 4
  %4 = add nsw i32 %3, 2
  store i32 %4, ptr %2, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z3barRi(ptr noundef %0) #0 {
  %t = alloca ptr, align 8
  store ptr %0, ptr %t, align 8
  %2 = load ptr, ptr %t, align 8
  %3 = load i32, ptr %2, align 4
  %4 = add nsw i32 %3, 3
  store i32 %4, ptr %2, align 4
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %t = alloca i32, align 4
  store i32 1, ptr %t, align 4
  call void @_Z3fooIRiEvRi(ptr noundef %t)
  %1 = load i32, ptr %t, align 4
  %2 = icmp eq i32 %1, 3
  br i1 %2, label %assert.exit.L13, label %assert.then.L13, !prof !5

assert.then.L13:                                  ; preds = %0
  %3 = call ptr @__acrt_iob_func(i32 2)
  %4 = call i32 (ptr, ptr, ...) @fprintf(ptr %3, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %0
  call void @_Z3barRi(ptr noundef %t)
  %5 = load i32, ptr %t, align 4
  %6 = icmp eq i32 %5, 6
  br i1 %6, label %assert.exit.L15, label %assert.then.L15, !prof !5

assert.then.L15:                                  ; preds = %assert.exit.L13
  %7 = call ptr @__acrt_iob_func(i32 2)
  %8 = call i32 (ptr, ptr, ...) @fprintf(ptr %7, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L13
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 0
}

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: nounwind
declare dso_local noundef ptr @__acrt_iob_func(i32 noundef) #3

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #4

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #5

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree }
attributes #3 = { nounwind }
attributes #4 = { cold noreturn nounwind }
attributes #5 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
