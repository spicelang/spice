; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 4
@anon.string.1 = private unnamed_addr constant [6 x i8] c"World\00", align 4
@anon.array.0 = private unnamed_addr constant [2 x ptr] [ptr @anon.string.0, ptr @anon.string.1]
@anon.string.2 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition '*iPtr == 13' evaluated to false.\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %test = alloca i32, align 4
  %i = alloca i32, align 4
  %iPtr = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_Z11printFormatIdEvd(double 1.123000e+00)
  call void @_Z11printFormatIiEvi(i32 543)
  call void @_Z11printFormatIA2_PKcEvA2_PKc([2 x ptr] [ptr @anon.string.0, ptr @anon.string.1])
  store i32 1234, ptr %test, align 4
  call void @_Z11printFormatIPiEvPi(ptr %test)
  store i32 12, ptr %i, align 4
  %1 = call ptr @_Z7getAIncIiEPiPi(ptr %i)
  store ptr %1, ptr %iPtr, align 8
  %2 = load ptr, ptr %iPtr, align 8
  %3 = load i32, ptr %2, align 4
  %4 = icmp eq i32 %3, 13
  br i1 %4, label %assert.exit.L12, label %assert.then.L12, !prof !5

assert.then.L12:                                  ; preds = %0
  %5 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %0
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

declare void @_Z11printFormatIdEvd(double)

declare void @_Z11printFormatIiEvi(i32)

declare void @_Z11printFormatIA2_PKcEvA2_PKc([2 x ptr])

declare void @_Z11printFormatIPiEvPi(ptr)

declare ptr @_Z7getAIncIiEPiPi(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline nounwind optnone uwtable }
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
