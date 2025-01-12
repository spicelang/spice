; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.TestStruct = type { i64 }

@anon.string.0 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'output.test == 125l' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'ts.test == 125l' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'ts.test == 123l' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'output.test == 123l' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

define private ptr @_Z16op.plusplus.postR10TestStruct(ptr %0) {
  %result = alloca ptr, align 8
  %ts = alloca ptr, align 8
  store ptr %0, ptr %ts, align 8
  %2 = load ptr, ptr %ts, align 8
  %test_addr = getelementptr inbounds %struct.TestStruct, ptr %2, i64 0, i32 0
  %3 = load i64, ptr %test_addr, align 8
  %4 = add nsw i64 %3, 1
  store i64 %4, ptr %test_addr, align 8
  %5 = load ptr, ptr %ts, align 8
  ret ptr %5
}

define private ptr @_Z18op.minusminus.postR10TestStruct(ptr %0) {
  %result = alloca ptr, align 8
  %ts = alloca ptr, align 8
  store ptr %0, ptr %ts, align 8
  %2 = load ptr, ptr %ts, align 8
  %test_addr = getelementptr inbounds %struct.TestStruct, ptr %2, i64 0, i32 0
  %3 = load i64, ptr %test_addr, align 8
  %4 = sub nsw i64 %3, 1
  store i64 %4, ptr %test_addr, align 8
  %5 = load ptr, ptr %ts, align 8
  ret ptr %5
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %ts = alloca %struct.TestStruct, align 8
  %output = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store %struct.TestStruct { i64 123 }, ptr %ts, align 8
  %1 = load %struct.TestStruct, ptr %ts, align 8
  %2 = call ptr @_Z16op.plusplus.postR10TestStruct(ptr %ts)
  store ptr %2, ptr %output, align 8
  %test_addr = getelementptr inbounds %struct.TestStruct, ptr %ts, i64 0, i32 0
  %3 = load i64, ptr %test_addr, align 8
  %4 = add nsw i64 %3, 1
  store i64 %4, ptr %test_addr, align 8
  %5 = load ptr, ptr %output, align 8
  %test_addr1 = getelementptr inbounds %struct.TestStruct, ptr %5, i64 0, i32 0
  %6 = load i64, ptr %test_addr1, align 8
  %7 = icmp eq i64 %6, 125
  br i1 %7, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %0
  %8 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %0
  %test_addr2 = getelementptr inbounds %struct.TestStruct, ptr %ts, i64 0, i32 0
  %9 = load i64, ptr %test_addr2, align 8
  %10 = icmp eq i64 %9, 125
  br i1 %10, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L19
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L19
  %test_addr3 = getelementptr inbounds %struct.TestStruct, ptr %ts, i64 0, i32 0
  %12 = load i64, ptr %test_addr3, align 8
  %13 = sub nsw i64 %12, 1
  store i64 %13, ptr %test_addr3, align 8
  %14 = load %struct.TestStruct, ptr %ts, align 8
  %15 = call ptr @_Z18op.minusminus.postR10TestStruct(ptr %ts)
  %test_addr4 = getelementptr inbounds %struct.TestStruct, ptr %ts, i64 0, i32 0
  %16 = load i64, ptr %test_addr4, align 8
  %17 = icmp eq i64 %16, 123
  br i1 %17, label %assert.exit.L23, label %assert.then.L23, !prof !0

assert.then.L23:                                  ; preds = %assert.exit.L20
  %18 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L20
  %19 = load ptr, ptr %output, align 8
  %test_addr5 = getelementptr inbounds %struct.TestStruct, ptr %19, i64 0, i32 0
  %20 = load i64, ptr %test_addr5, align 8
  %21 = icmp eq i64 %20, 123
  br i1 %21, label %assert.exit.L24, label %assert.then.L24, !prof !0

assert.then.L24:                                  ; preds = %assert.exit.L23
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L23
  %23 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %24 = load i32, ptr %result, align 4
  ret i32 %24
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
