; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.NumberIterator = type { %interface.IIterator, i16, i16, i16 }
%interface.IIterator = type { ptr }
%struct.NumberIterator.0 = type { %interface.IIterator, i64, i64, i64 }

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %shortIterator = alloca %struct.NumberIterator, align 8
  %1 = alloca %struct.NumberIterator.0, align 8
  %2 = tail call %struct.NumberIterator @_Z5rangeIsE14NumberIteratorIsEss(i16 3, i16 8) #2
  %.fca.0.0.extract2 = extractvalue %struct.NumberIterator %2, 0, 0
  store ptr %.fca.0.0.extract2, ptr %shortIterator, align 8
  %.fca.1.extract4 = extractvalue %struct.NumberIterator %2, 1
  %.fca.1.gep5 = getelementptr inbounds nuw i8, ptr %shortIterator, i64 8
  store i16 %.fca.1.extract4, ptr %.fca.1.gep5, align 8
  %.fca.2.extract6 = extractvalue %struct.NumberIterator %2, 2
  %.fca.2.gep7 = getelementptr inbounds nuw i8, ptr %shortIterator, i64 10
  store i16 %.fca.2.extract6, ptr %.fca.2.gep7, align 2
  %.fca.3.extract8 = extractvalue %struct.NumberIterator %2, 3
  %.fca.3.gep9 = getelementptr inbounds nuw i8, ptr %shortIterator, i64 12
  store i16 %.fca.3.extract8, ptr %.fca.3.gep9, align 4
  %3 = call i1 @_ZN14NumberIteratorIsE7isValidEv(ptr nonnull %shortIterator) #2
  br i1 %3, label %foreach.body.L5.lr.ph, label %foreach.exit.L5

foreach.body.L5.lr.ph:                            ; preds = %0
  %.fca.1.gep = getelementptr inbounds nuw i8, ptr %1, i64 8
  %.fca.2.gep = getelementptr inbounds nuw i8, ptr %1, i64 16
  %.fca.3.gep = getelementptr inbounds nuw i8, ptr %1, i64 24
  br label %foreach.body.L5

foreach.body.L5:                                  ; preds = %foreach.body.L5.lr.ph, %foreach.tail.L5
  %4 = call ptr @_ZN14NumberIteratorIsE3getEv(ptr nonnull %shortIterator) #2
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %6)
  %8 = and i16 %5, 1
  %.not = icmp eq i16 %8, 0
  br i1 %.not, label %foreach.tail.L5, label %if.then.L7

if.then.L7:                                       ; preds = %foreach.body.L5
  %9 = call %struct.NumberIterator.0 @_Z5rangeIlE14NumberIteratorIlEll(i64 1, i64 2) #2
  %.fca.0.0.extract = extractvalue %struct.NumberIterator.0 %9, 0, 0
  store ptr %.fca.0.0.extract, ptr %1, align 8
  %.fca.1.extract = extractvalue %struct.NumberIterator.0 %9, 1
  store i64 %.fca.1.extract, ptr %.fca.1.gep, align 8
  %.fca.2.extract = extractvalue %struct.NumberIterator.0 %9, 2
  store i64 %.fca.2.extract, ptr %.fca.2.gep, align 8
  %.fca.3.extract = extractvalue %struct.NumberIterator.0 %9, 3
  store i64 %.fca.3.extract, ptr %.fca.3.gep, align 8
  %10 = call i1 @_ZN14NumberIteratorIlE7isValidEv(ptr nonnull %1) #2
  br i1 %10, label %foreach.body.L8, label %foreach.tail.L5

foreach.body.L8:                                  ; preds = %if.then.L7
  %11 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %1) #2
  %12 = load i64, ptr %11, align 8
  %13 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i64 %12)
  br label %foreach.tail.L5

foreach.tail.L5:                                  ; preds = %foreach.body.L5, %if.then.L7, %foreach.body.L8
  call void @_ZN14NumberIteratorIsE4nextEv(ptr nonnull %shortIterator) #2
  %14 = call i1 @_ZN14NumberIteratorIsE7isValidEv(ptr nonnull %shortIterator) #2
  br i1 %14, label %foreach.body.L5, label %foreach.exit.L5

foreach.exit.L5:                                  ; preds = %foreach.tail.L5, %0
  %15 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  ret i32 0
}

declare %struct.NumberIterator @_Z5rangeIsE14NumberIteratorIsEss(i16, i16) local_unnamed_addr

declare i1 @_ZN14NumberIteratorIsE7isValidEv(ptr) local_unnamed_addr

declare ptr @_ZN14NumberIteratorIsE3getEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare %struct.NumberIterator.0 @_Z5rangeIlE14NumberIteratorIlEll(i64, i64) local_unnamed_addr

declare i1 @_ZN14NumberIteratorIlE7isValidEv(ptr) local_unnamed_addr

declare ptr @_ZN14NumberIteratorIlE3getEv(ptr) local_unnamed_addr

declare void @_ZN14NumberIteratorIsE4nextEv(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
