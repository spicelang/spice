; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.TestStruct = type { i64 }

@anon.string.0 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'output.test == 125l' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'ts.test == 125l' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'ts.test == 123l' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'output.test == 123l' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 4

define private noundef ptr @_Z16op.plusplus.postR10TestStruct(ptr noundef %0) {
  %result = alloca ptr, align 8
  %ts = alloca ptr, align 8
  store ptr %0, ptr %ts, align 8
  %2 = load ptr, ptr %ts, align 8
  %test.addr = getelementptr inbounds %struct.TestStruct, ptr %2, i64 0, i32 0
  %3 = load i64, ptr %test.addr, align 8
  %4 = add nsw i64 %3, 1
  store i64 %4, ptr %test.addr, align 8
  %5 = load ptr, ptr %ts, align 8
  ret ptr %5
}

define private noundef ptr @_Z18op.minusminus.postR10TestStruct(ptr noundef %0) {
  %result = alloca ptr, align 8
  %ts = alloca ptr, align 8
  store ptr %0, ptr %ts, align 8
  %2 = load ptr, ptr %ts, align 8
  %test.addr = getelementptr inbounds %struct.TestStruct, ptr %2, i64 0, i32 0
  %3 = load i64, ptr %test.addr, align 8
  %4 = sub nsw i64 %3, 1
  store i64 %4, ptr %test.addr, align 8
  %5 = load ptr, ptr %ts, align 8
  ret ptr %5
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %ts = alloca %struct.TestStruct, align 8
  %output = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store %struct.TestStruct { i64 123 }, ptr %ts, align 8
  %1 = load %struct.TestStruct, ptr %ts, align 8
  %2 = call ptr @_Z16op.plusplus.postR10TestStruct(ptr %ts)
  store ptr %2, ptr %output, align 8
  %test.addr = getelementptr inbounds %struct.TestStruct, ptr %ts, i64 0, i32 0
  %3 = load i64, ptr %test.addr, align 8
  %4 = add nsw i64 %3, 1
  store i64 %4, ptr %test.addr, align 8
  %5 = load ptr, ptr %output, align 8
  %test.addr1 = getelementptr inbounds %struct.TestStruct, ptr %5, i64 0, i32 0
  %6 = load i64, ptr %test.addr1, align 8
  %7 = icmp eq i64 %6, 125
  br i1 %7, label %assert.exit.L19, label %assert.then.L19, !prof !5

assert.then.L19:                                  ; preds = %0
  %8 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %0
  %test.addr2 = getelementptr inbounds %struct.TestStruct, ptr %ts, i64 0, i32 0
  %9 = load i64, ptr %test.addr2, align 8
  %10 = icmp eq i64 %9, 125
  br i1 %10, label %assert.exit.L20, label %assert.then.L20, !prof !5

assert.then.L20:                                  ; preds = %assert.exit.L19
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L19
  %test.addr3 = getelementptr inbounds %struct.TestStruct, ptr %ts, i64 0, i32 0
  %12 = load i64, ptr %test.addr3, align 8
  %13 = sub nsw i64 %12, 1
  store i64 %13, ptr %test.addr3, align 8
  %14 = load %struct.TestStruct, ptr %ts, align 8
  %15 = call ptr @_Z18op.minusminus.postR10TestStruct(ptr %ts)
  %test.addr4 = getelementptr inbounds %struct.TestStruct, ptr %ts, i64 0, i32 0
  %16 = load i64, ptr %test.addr4, align 8
  %17 = icmp eq i64 %16, 123
  br i1 %17, label %assert.exit.L23, label %assert.then.L23, !prof !5

assert.then.L23:                                  ; preds = %assert.exit.L20
  %18 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L20
  %19 = load ptr, ptr %output, align 8
  %test.addr5 = getelementptr inbounds %struct.TestStruct, ptr %19, i64 0, i32 0
  %20 = load i64, ptr %test.addr5, align 8
  %21 = icmp eq i64 %20, 123
  br i1 %21, label %assert.exit.L24, label %assert.then.L24, !prof !5

assert.then.L24:                                  ; preds = %assert.exit.L23
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L23
  %23 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %24 = load i32, ptr %result, align 4
  ret i32 %24
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
!5 = !{!"branch_weights", i32 1048575, i32 1}
