; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Struct = type { ptr, i1 }

@printf.str.0 = private unnamed_addr constant [20 x i8] c"From procedure: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [19 x i8] c"From function: %d\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 'i == -4309' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'd == -107.64' evaluated to false.\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 4

define private void @_Z4procRiRK6Struct(ptr %0, ptr %1) {
  %intRef = alloca ptr, align 8
  %structRef = alloca ptr, align 8
  store ptr %0, ptr %intRef, align 8
  store ptr %1, ptr %structRef, align 8
  %3 = load ptr, ptr %intRef, align 8
  %4 = load i32, ptr %3, align 4
  %5 = add nsw i32 %4, 12
  store i32 %5, ptr %3, align 4
  %6 = load ptr, ptr %structRef, align 8
  %ref.addr = getelementptr inbounds %struct.Struct, ptr %6, i64 0, i32 0
  %7 = load ptr, ptr %ref.addr, align 8
  %8 = load i32, ptr %7, align 4
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %8)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

define private i32 @_Z4funcRdRK6Struct(ptr %0, ptr %1) {
  %result = alloca i32, align 4
  %doubleRef = alloca ptr, align 8
  %structRef = alloca ptr, align 8
  store ptr %0, ptr %doubleRef, align 8
  store ptr %1, ptr %structRef, align 8
  %3 = load ptr, ptr %doubleRef, align 8
  %4 = load double, ptr %3, align 8
  %5 = fmul double %4, -1.560000e+00
  store double %5, ptr %3, align 8
  %6 = load ptr, ptr %structRef, align 8
  %b.addr = getelementptr inbounds %struct.Struct, ptr %6, i64 0, i32 1
  %7 = load i1, ptr %b.addr, align 1
  %8 = zext i1 %7 to i32
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %8)
  ret i32 0
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %1 = alloca %struct.Struct, align 8
  %d = alloca double, align 8
  %2 = alloca %struct.Struct, align 8
  store i32 0, ptr %result, align 4
  store i32 -4321, ptr %i, align 4
  store ptr %i, ptr %1, align 8
  %3 = getelementptr inbounds nuw %struct.Struct, ptr %1, i32 0, i32 1
  store i1 true, ptr %3, align 1
  call void @_Z4procRiRK6Struct(ptr %i, ptr %1)
  %4 = load i32, ptr %i, align 4
  %5 = icmp eq i32 %4, -4309
  br i1 %5, label %assert.exit.L20, label %assert.then.L20, !prof !5

assert.then.L20:                                  ; preds = %0
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %0
  store double 6.900000e+01, ptr %d, align 8
  store ptr %i, ptr %2, align 8
  %7 = getelementptr inbounds nuw %struct.Struct, ptr %2, i32 0, i32 1
  store i1 false, ptr %7, align 1
  %8 = call i32 @_Z4funcRdRK6Struct(ptr %d, ptr %2)
  store i32 %8, ptr %result, align 4
  %9 = load double, ptr %d, align 8
  %10 = fcmp oeq double %9, -1.076400e+02
  br i1 %10, label %assert.exit.L24, label %assert.then.L24, !prof !5

assert.then.L24:                                  ; preds = %assert.exit.L20
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L20
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  %13 = load i32, ptr %result, align 4
  ret i32 %13
}

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { nofree nounwind }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 2000, i32 1}
