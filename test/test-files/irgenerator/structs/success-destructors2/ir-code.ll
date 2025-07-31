; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Vector = type { i1, ptr }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Test\00", align 4
@printf.str.0 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Destructor called!\00", align 4
@anon.string.1 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'this.field1 == true' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [5 x i8] c"Test\00", align 4
@anon.string.3 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 'this.field2 == \22Test\22' evaluated to false.\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %vec = alloca %struct.Vector, align 8
  store i32 0, ptr %result, align 4
  store %struct.Vector { i1 true, ptr @anon.string.0 }, ptr %vec, align 8
  %field1.addr = getelementptr inbounds %struct.Vector, ptr %vec, i64 0, i32 0
  %1 = load i1, ptr %field1.addr, align 1
  %2 = zext i1 %1 to i32
  %field2.addr = getelementptr inbounds %struct.Vector, ptr %vec, i64 0, i32 1
  %3 = load ptr, ptr %field2.addr, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2, ptr %3)
  call void @_ZN6Vector4dtorEv(ptr %vec)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

define private void @_ZN6Vector4dtorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  %2 = alloca i1, align 1
  store ptr %0, ptr %this, align 8
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  %4 = load ptr, ptr %this, align 8
  %field1.addr = getelementptr inbounds %struct.Vector, ptr %4, i64 0, i32 0
  store i1 true, ptr %2, align 1
  %5 = call i32 @memcmp(ptr %field1.addr, ptr %2, i64 0)
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %assert.exit.L13, label %assert.then.L13, !prof !5

assert.then.L13:                                  ; preds = %1
  %7 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %1
  %8 = load ptr, ptr %this, align 8
  %field2.addr = getelementptr inbounds %struct.Vector, ptr %8, i64 0, i32 1
  %9 = load ptr, ptr %field2.addr, align 8
  %10 = call i1 @_Z10isRawEqualPKcPKc(ptr %9, ptr @anon.string.2)
  br i1 %10, label %assert.exit.L14, label %assert.then.L14, !prof !5

assert.then.L14:                                  ; preds = %assert.exit.L13
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  ret void
}

; Function Attrs: nounwind
declare i32 @memcmp(ptr, ptr, i64) #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

declare i1 @_Z10isRawEqualPKcPKc(ptr, ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
attributes #3 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 2000, i32 1}
