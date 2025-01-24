; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.NumberIterator = type { %interface.IIterator, i16, i16, i16 }
%interface.IIterator = type { ptr }
%struct.NumberIterator.0 = type { %interface.IIterator, i64, i64, i64 }

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %shortIterator = alloca %struct.NumberIterator, align 8
  %s = alloca i16, align 2
  %1 = alloca %struct.NumberIterator.0, align 8
  %l = alloca ptr, align 8
  %2 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  %3 = call %struct.NumberIterator @_Z5rangeIsE14NumberIteratorIsEss(i16 3, i16 8)
  store %struct.NumberIterator %3, ptr %shortIterator, align 8
  br label %foreach.head.L5

foreach.head.L5:                                  ; preds = %foreach.tail.L5, %0
  %4 = call i1 @_ZN14NumberIteratorIsE7isValidEv(ptr %shortIterator)
  br i1 %4, label %foreach.body.L5, label %foreach.exit.L5

foreach.body.L5:                                  ; preds = %foreach.head.L5
  %5 = call ptr @_ZN14NumberIteratorIsE3getEv(ptr %shortIterator)
  %6 = load i16, ptr %5, align 2
  store i16 %6, ptr %s, align 2
  %7 = load i16, ptr %s, align 2
  %8 = sext i16 %7 to i32
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %8)
  %10 = load i16, ptr %s, align 2
  %11 = and i16 %10, 1
  %12 = sext i16 %11 to i32
  %13 = icmp eq i32 %12, 1
  br i1 %13, label %if.then.L7, label %if.exit.L7

if.then.L7:                                       ; preds = %foreach.body.L5
  %14 = call %struct.NumberIterator.0 @_Z5rangeIlE14NumberIteratorIlEll(i64 1, i64 2)
  store %struct.NumberIterator.0 %14, ptr %1, align 8
  br label %foreach.head.L8

foreach.head.L8:                                  ; preds = %foreach.tail.L8, %if.then.L7
  %15 = call i1 @_ZN14NumberIteratorIlE7isValidEv(ptr %1)
  br i1 %15, label %foreach.body.L8, label %foreach.exit.L8

foreach.body.L8:                                  ; preds = %foreach.head.L8
  %16 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr %1)
  store ptr %16, ptr %2, align 8
  %17 = load ptr, ptr %2, align 8
  %18 = load i64, ptr %17, align 8
  %19 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 %18)
  br label %foreach.tail.L5

foreach.tail.L8:                                  ; No predecessors!
  call void @_ZN14NumberIteratorIlE4nextEv(ptr %1)
  br label %foreach.head.L8

foreach.exit.L8:                                  ; preds = %foreach.head.L8
  br label %if.exit.L7

if.exit.L7:                                       ; preds = %foreach.exit.L8, %foreach.body.L5
  br label %foreach.tail.L5

foreach.tail.L5:                                  ; preds = %if.exit.L7, %foreach.body.L8
  call void @_ZN14NumberIteratorIsE4nextEv(ptr %shortIterator)
  br label %foreach.head.L5

foreach.exit.L5:                                  ; preds = %foreach.head.L5
  %20 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  %21 = load i32, ptr %result, align 4
  ret i32 %21
}

declare %struct.NumberIterator @_Z5rangeIsE14NumberIteratorIsEss(i16, i16)

declare i1 @_ZN14NumberIteratorIsE7isValidEv(ptr)

declare ptr @_ZN14NumberIteratorIsE3getEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

declare %struct.NumberIterator.0 @_Z5rangeIlE14NumberIteratorIlEll(i64, i64)

declare i1 @_ZN14NumberIteratorIlE7isValidEv(ptr)

declare ptr @_ZN14NumberIteratorIlE3getEv(ptr)

declare void @_ZN14NumberIteratorIlE4nextEv(ptr)

declare void @_ZN14NumberIteratorIsE4nextEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
