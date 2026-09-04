; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 4
@anon.string.1 = private unnamed_addr constant [6 x i8] c"World\00", align 4
@anon.array.0 = private unnamed_addr constant [2 x ptr] [ptr @anon.string.0, ptr @anon.string.1]
@anon.string.2 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition '*iPtr == 13' evaluated to false.\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %arg.decay = alloca [2 x ptr], align 8
  %test = alloca i32, align 4
  %i = alloca i32, align 4
  %iPtr = alloca ptr, align 8
  call void @_Z11printFormatIdEvd(double noundef 1.123000e+00)
  call void @_Z11printFormatIiEvi(i32 noundef 543)
  call void @llvm.memcpy.p0.p0.i64(ptr %arg.decay, ptr @anon.array.0, i64 16, i1 false)
  call void @_Z11printFormatIA2_PKcEvA2_PKc(ptr noundef %arg.decay)
  store i32 1234, ptr %test, align 4
  call void @_Z11printFormatIPiEvPi(ptr noundef align 4 dereferenceable(4) %test)
  store i32 12, ptr %i, align 4
  %1 = call noundef ptr @_Z7getAIncIiEPiPi(ptr noundef align 4 dereferenceable(4) %i)
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
  ret i32 0
}

declare void @_Z11printFormatIdEvd(double)

declare void @_Z11printFormatIiEvi(i32)

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #1

declare void @_Z11printFormatIA2_PKcEvA2_PKc(ptr)

declare void @_Z11printFormatIPiEvPi(ptr)

declare ptr @_Z7getAIncIiEPiPi(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nofree nounwind }
attributes #3 = { cold noreturn nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
