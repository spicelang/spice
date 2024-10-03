; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'fruit == fruit2' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

define private i32 @_Z3foo5Fruit(i32 %0) {
  %result = alloca i32, align 4
  %fruit = alloca i32, align 4
  store i32 %0, ptr %fruit, align 4
  %2 = load i32, ptr %fruit, align 4
  ret i32 %2
}

define private ptr @_Z3barRK5Fruit(ptr %0) {
  %result = alloca ptr, align 8
  %fruit = alloca ptr, align 8
  store ptr %0, ptr %fruit, align 8
  %2 = load ptr, ptr %fruit, align 8
  ret ptr %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %fruit = alloca i32, align 4
  %1 = alloca i32, align 4
  %fruit2 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 2, ptr %fruit, align 4
  %2 = load i32, ptr %fruit, align 4
  %3 = call i32 @_Z3foo5Fruit(i32 %2)
  store i32 %3, ptr %1, align 4
  %4 = call ptr @_Z3barRK5Fruit(ptr %1)
  %5 = load i32, ptr %4, align 4
  store i32 %5, ptr %fruit2, align 4
  %6 = load i32, ptr %fruit2, align 4
  %7 = load i32, ptr %fruit, align 4
  %8 = icmp eq i32 %7, %6
  br i1 %8, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %0
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %0
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %11 = load i32, ptr %result, align 4
  ret i32 %11
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
