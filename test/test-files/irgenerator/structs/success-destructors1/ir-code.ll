; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Vector = type { i1, ptr }

@printf.str.0 = private unnamed_addr constant [19 x i8] c"Destructor called!\00", align 1
@anon.string.0 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'this.field1 == true' evaluated to false.\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@anon.string.2 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 'this.field2 == \22Test\22' evaluated to false.\0A\00", align 1
@anon.string.3 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1

define private void @_ZN6Vector4dtorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  %2 = alloca i1, align 1
  store ptr %0, ptr %this, align 8
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %4 = load ptr, ptr %this, align 8
  %field1.addr = getelementptr inbounds %struct.Vector, ptr %4, i64 0, i32 0
  store i1 true, ptr %2, align 1
  %5 = call i32 @memcmp(ptr %field1.addr, ptr %2, i64 0)
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %assert.exit.L8, label %assert.then.L8, !prof !5

assert.then.L8:                                   ; preds = %1
  %7 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L8:                                   ; preds = %1
  %8 = load ptr, ptr %this, align 8
  %field2.addr = getelementptr inbounds %struct.Vector, ptr %8, i64 0, i32 1
  %9 = load ptr, ptr %field2.addr, align 8
  %10 = call i1 @_Z10isRawEqualPKcPKc(ptr %9, ptr @anon.string.1)
  br i1 %10, label %assert.exit.L9, label %assert.then.L9, !prof !5

assert.then.L9:                                   ; preds = %assert.exit.L8
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %assert.exit.L8
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: nounwind
declare i32 @memcmp(ptr, ptr, i64) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

declare i1 @_Z10isRawEqualPKcPKc(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #3 {
  %result = alloca i32, align 4
  %vec = alloca %struct.Vector, align 8
  store i32 0, ptr %result, align 4
  store %struct.Vector { i1 true, ptr @anon.string.3 }, ptr %vec, align 8
  %field1.addr = getelementptr inbounds %struct.Vector, ptr %vec, i64 0, i32 0
  %1 = load i1, ptr %field1.addr, align 1
  %2 = zext i1 %1 to i32
  %field2.addr = getelementptr inbounds %struct.Vector, ptr %vec, i64 0, i32 1
  %3 = load ptr, ptr %field2.addr, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %2, ptr %3)
  call void @_ZN6Vector4dtorEv(ptr %vec)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

attributes #0 = { nofree nounwind }
attributes #1 = { nounwind }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 2000, i32 1}
