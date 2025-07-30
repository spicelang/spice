; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [58 x i8] c"Assertion failed: Condition 'x == 6' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition 'l2(x)' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [59 x i8] c"Assertion failed: Condition 'x == 11' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [19 x i8] c"All tests passed!\0A\00", align 4

define private void @_Z4testPFCvRiEPFCbRiE({ ptr, ptr } %0, { ptr, ptr } %1) {
  %l1 = alloca { ptr, ptr }, align 8
  %l2 = alloca { ptr, ptr }, align 8
  %x = alloca i32, align 4
  store { ptr, ptr } %0, ptr %l1, align 8
  store { ptr, ptr } %1, ptr %l2, align 8
  store i32 1, ptr %x, align 4
  %3 = getelementptr inbounds nuw { ptr, ptr }, ptr %l1, i32 0, i32 1
  %captures = load ptr, ptr %3, align 8
  %fct = load ptr, ptr %l1, align 8
  call void %fct(ptr %captures, ptr %x)
  %4 = load i32, ptr %x, align 4
  %5 = icmp eq i32 %4, 6
  br i1 %5, label %assert.exit.L4, label %assert.then.L4, !prof !5

assert.then.L4:                                   ; preds = %2
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L4:                                   ; preds = %2
  %7 = getelementptr inbounds nuw { ptr, ptr }, ptr %l2, i32 0, i32 1
  %captures1 = load ptr, ptr %7, align 8
  %fct2 = load ptr, ptr %l2, align 8
  %8 = call i1 %fct2(ptr %captures1, ptr %x)
  br i1 %8, label %assert.exit.L5, label %assert.then.L5, !prof !5

assert.then.L5:                                   ; preds = %assert.exit.L4
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L5:                                   ; preds = %assert.exit.L4
  %10 = load i32, ptr %x, align 4
  %11 = icmp eq i32 %10, 11
  br i1 %11, label %assert.exit.L6, label %assert.then.L6, !prof !5

assert.then.L6:                                   ; preds = %assert.exit.L5
  %12 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L6:                                   ; preds = %assert.exit.L5
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #0

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #2 {
  %result = alloca i32, align 4
  %z = alloca i32, align 4
  %w = alloca i32, align 4
  %captures = alloca { i32, i32 }, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %foo1 = alloca { ptr, ptr }, align 8
  %captures1 = alloca { i32, i32 }, align 8
  %fat.ptr2 = alloca { ptr, ptr }, align 8
  %foo2 = alloca { ptr, ptr }, align 8
  store i32 0, ptr %result, align 4
  store i32 2, ptr %z, align 4
  store i32 3, ptr %w, align 4
  %1 = load i32, ptr %w, align 4
  store i32 %1, ptr %captures, align 4
  %2 = load i32, ptr %z, align 4
  %3 = getelementptr inbounds nuw { i32, i32 }, ptr %captures, i32 0, i32 1
  store i32 %2, ptr %3, align 4
  store ptr @_Z15lambda.L12C20.0Ri, ptr %fat.ptr, align 8
  %4 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr %captures, ptr %4, align 8
  %5 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %5, ptr %foo1, align 8
  %6 = load i32, ptr %w, align 4
  store i32 %6, ptr %captures1, align 4
  %7 = load i32, ptr %z, align 4
  %8 = getelementptr inbounds nuw { i32, i32 }, ptr %captures1, i32 0, i32 1
  store i32 %7, ptr %8, align 4
  store ptr @_Z15lambda.L15C26.0Ri, ptr %fat.ptr2, align 8
  %9 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr2, i32 0, i32 1
  store ptr %captures1, ptr %9, align 8
  %10 = load { ptr, ptr }, ptr %fat.ptr2, align 8
  store { ptr, ptr } %10, ptr %foo2, align 8
  %11 = load { ptr, ptr }, ptr %foo1, align 8
  %12 = load { ptr, ptr }, ptr %foo2, align 8
  call void @_Z4testPFCvRiEPFCbRiE({ ptr, ptr } %11, { ptr, ptr } %12)
  %13 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %14 = load i32, ptr %result, align 4
  ret i32 %14
}

define private void @_Z15lambda.L12C20.0Ri(ptr noundef nonnull dereferenceable(8) %0, ptr %1) {
  %captures = alloca ptr, align 8
  %x = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  store ptr %1, ptr %x, align 8
  %3 = load ptr, ptr %captures, align 8
  %z = getelementptr inbounds nuw { i32, i32 }, ptr %3, i32 0, i32 1
  %4 = load i32, ptr %z, align 4
  %5 = load i32, ptr %3, align 4
  %6 = add nsw i32 %4, %5
  %7 = load ptr, ptr %x, align 8
  %8 = load i32, ptr %7, align 4
  %9 = add nsw i32 %8, %6
  store i32 %9, ptr %7, align 4
  ret void
}

define private i1 @_Z15lambda.L15C26.0Ri(ptr noundef nonnull dereferenceable(8) %0, ptr %1) {
  %result = alloca i1, align 1
  %captures = alloca ptr, align 8
  %x = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8
  store ptr %1, ptr %x, align 8
  %3 = load ptr, ptr %captures, align 8
  %z = getelementptr inbounds nuw { i32, i32 }, ptr %3, i32 0, i32 1
  %4 = load i32, ptr %z, align 4
  %5 = load i32, ptr %3, align 4
  %6 = add nsw i32 %4, %5
  %7 = load ptr, ptr %x, align 8
  %8 = load i32, ptr %7, align 4
  %9 = add nsw i32 %8, %6
  store i32 %9, ptr %7, align 4
  ret i1 true
}

attributes #0 = { nofree nounwind }
attributes #1 = { cold noreturn nounwind }
attributes #2 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 2000, i32 1}
