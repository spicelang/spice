; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'x == 6' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [59 x i8] c"Assertion failed: Condition 'foo2(x)' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [59 x i8] c"Assertion failed: Condition 'x == 11' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [19 x i8] c"All tests passed!\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %z = alloca i32, align 4
  %w = alloca i32, align 4
  %captures = alloca { i32, i32 }, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %foo1 = alloca { ptr, ptr, i64 }, align 8
  %captures1 = alloca { i32, i32 }, align 8
  %fat.ptr2 = alloca { ptr, ptr, i64 }, align 8
  %foo2 = alloca { ptr, ptr, i64 }, align 8
  %x = alloca i32, align 4
  store i32 2, ptr %z, align 4
  store i32 3, ptr %w, align 4
  %1 = load i32, ptr %w, align 4
  store i32 %1, ptr %captures, align 4
  %2 = load i32, ptr %z, align 4
  %3 = getelementptr inbounds nuw { i32, i32 }, ptr %captures, i32 0, i32 1
  store i32 %2, ptr %3, align 4
  store ptr @_Z14lambda.L4C20.0Ri, ptr %fat.ptr, align 8
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %4, align 8
  %5 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2
  store i64 8, ptr %5, align 8
  %6 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8
  store { ptr, ptr, i64 } %6, ptr %foo1, align 8
  %7 = load i32, ptr %w, align 4
  store i32 %7, ptr %captures1, align 4
  %8 = load i32, ptr %z, align 4
  %9 = getelementptr inbounds nuw { i32, i32 }, ptr %captures1, i32 0, i32 1
  store i32 %8, ptr %9, align 4
  store ptr @_Z14lambda.L7C26.0Ri, ptr %fat.ptr2, align 8
  %10 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 1
  store ptr %captures1, ptr %10, align 8
  %11 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 2
  store i64 8, ptr %11, align 8
  %12 = load { ptr, ptr, i64 }, ptr %fat.ptr2, align 8
  store { ptr, ptr, i64 } %12, ptr %foo2, align 8
  store i32 1, ptr %x, align 4
  %13 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %foo1, i32 0, i32 1
  %captures3 = load ptr, ptr %13, align 8
  %fct = load ptr, ptr %foo1, align 8
  call void %fct(ptr %x, ptr %captures3)
  %14 = load i32, ptr %x, align 4
  %15 = icmp eq i32 %14, 6
  br i1 %15, label %assert.exit.L13, label %assert.then.L13, !prof !5

assert.then.L13:                                  ; preds = %0
  %16 = call ptr @__acrt_iob_func(i32 2)
  %17 = call i32 (ptr, ptr, ...) @fprintf(ptr %16, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %0
  %18 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %foo2, i32 0, i32 1
  %captures4 = load ptr, ptr %18, align 8
  %fct5 = load ptr, ptr %foo2, align 8
  %19 = call i1 %fct5(ptr %x, ptr %captures4)
  br i1 %19, label %assert.exit.L14, label %assert.then.L14, !prof !5

assert.then.L14:                                  ; preds = %assert.exit.L13
  %20 = call ptr @__acrt_iob_func(i32 2)
  %21 = call i32 (ptr, ptr, ...) @fprintf(ptr %20, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %22 = load i32, ptr %x, align 4
  %23 = icmp eq i32 %22, 11
  br i1 %23, label %assert.exit.L15, label %assert.then.L15, !prof !5

assert.then.L15:                                  ; preds = %assert.exit.L14
  %24 = call ptr @__acrt_iob_func(i32 2)
  %25 = call i32 (ptr, ptr, ...) @fprintf(ptr %24, ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %26 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z14lambda.L4C20.0Ri(ptr %0, ptr noundef nonnull dereferenceable(8) %1) #1 {
  %x = alloca ptr, align 8
  %captures = alloca ptr, align 8
  store ptr %0, ptr %x, align 8
  store ptr %1, ptr %captures, align 8
  %3 = load ptr, ptr %captures, align 8
  %z = getelementptr inbounds nuw { i32, i32 }, ptr %3, i32 0, i32 1
  %4 = load i32, ptr %3, align 4
  %5 = load i32, ptr %z, align 4
  %6 = add nsw i32 %5, %4
  %7 = load ptr, ptr %x, align 8
  %8 = load i32, ptr %7, align 4
  %9 = add nsw i32 %8, %6
  store i32 %9, ptr %7, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i1 @_Z14lambda.L7C26.0Ri(ptr %0, ptr noundef nonnull dereferenceable(8) %1) #1 {
  %x = alloca ptr, align 8
  %captures = alloca ptr, align 8
  store ptr %0, ptr %x, align 8
  store ptr %1, ptr %captures, align 8
  %3 = load ptr, ptr %captures, align 8
  %z = getelementptr inbounds nuw { i32, i32 }, ptr %3, i32 0, i32 1
  %4 = load i32, ptr %3, align 4
  %5 = load i32, ptr %z, align 4
  %6 = add nsw i32 %5, %4
  %7 = load ptr, ptr %x, align 8
  %8 = load i32, ptr %7, align 4
  %9 = add nsw i32 %8, %6
  store i32 %9, ptr %7, align 4
  ret i1 true
}

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: nounwind
declare dso_local noundef ptr @__acrt_iob_func(i32 noundef) #3

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #4

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #5

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { noinline nounwind optnone uwtable }
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
