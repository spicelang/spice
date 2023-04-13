; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__short__NumberIterator__short_short_short = type { i16, i16, i16 }
%__long__NumberIterator__long_long_long = type { i64, i64, i64 }

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %shortIterator = alloca %__short__NumberIterator__short_short_short, align 8
  %1 = alloca %__long__NumberIterator__long_long_long, align 8
  %2 = tail call %__short__NumberIterator__short_short_short @"_f__void__NumberIterator<short>__range__short_short"(i16 3, i16 8) #2
  %.fca.0.extract2 = extractvalue %__short__NumberIterator__short_short_short %2, 0
  %.fca.1.extract4 = extractvalue %__short__NumberIterator__short_short_short %2, 1
  %.fca.1.gep5 = getelementptr inbounds %__short__NumberIterator__short_short_short, ptr %shortIterator, i64 0, i32 1
  %.fca.2.extract6 = extractvalue %__short__NumberIterator__short_short_short %2, 2
  %.fca.2.gep7 = getelementptr inbounds %__short__NumberIterator__short_short_short, ptr %shortIterator, i64 0, i32 2
  store i16 %.fca.0.extract2, ptr %shortIterator, align 8
  store i16 %.fca.1.extract4, ptr %.fca.1.gep5, align 2
  store i16 %.fca.2.extract6, ptr %.fca.2.gep7, align 4
  %3 = call i1 @_mf__NumberIterator_short__bool__isValid(ptr nonnull %shortIterator) #2
  br i1 %3, label %foreach.body.L5.lr.ph, label %foreach.exit.L5

foreach.body.L5.lr.ph:                            ; preds = %0
  %.fca.1.gep = getelementptr inbounds %__long__NumberIterator__long_long_long, ptr %1, i64 0, i32 1
  %.fca.2.gep = getelementptr inbounds %__long__NumberIterator__long_long_long, ptr %1, i64 0, i32 2
  br label %foreach.body.L5

foreach.body.L5:                                  ; preds = %foreach.body.L5.lr.ph, %foreach.tail.L5
  %4 = call ptr @_mf__NumberIterator_short__shortref__get(ptr nonnull %shortIterator) #2
  %5 = load i16, ptr %4, align 2
  %6 = sext i16 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %6)
  %8 = and i16 %5, 1
  %.not = icmp eq i16 %8, 0
  br i1 %.not, label %foreach.tail.L5, label %if.then.L7

if.then.L7:                                       ; preds = %foreach.body.L5
  %9 = call %__long__NumberIterator__long_long_long @"_f__void__NumberIterator<long>__range__long_long"(i64 1, i64 2) #2
  %.fca.0.extract = extractvalue %__long__NumberIterator__long_long_long %9, 0
  store i64 %.fca.0.extract, ptr %1, align 8
  %.fca.1.extract = extractvalue %__long__NumberIterator__long_long_long %9, 1
  store i64 %.fca.1.extract, ptr %.fca.1.gep, align 8
  %.fca.2.extract = extractvalue %__long__NumberIterator__long_long_long %9, 2
  store i64 %.fca.2.extract, ptr %.fca.2.gep, align 8
  %10 = call i1 @_mf__NumberIterator_long__bool__isValid(ptr nonnull %1) #2
  br i1 %10, label %foreach.body.L8, label %foreach.tail.L5

foreach.body.L8:                                  ; preds = %if.then.L7
  %11 = call ptr @_mf__NumberIterator_long__longref__get(ptr nonnull %1) #2
  %12 = load i64, ptr %11, align 8
  %13 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i64 %12)
  br label %foreach.tail.L5

foreach.tail.L5:                                  ; preds = %foreach.body.L5, %if.then.L7, %foreach.body.L8
  call void @_mp__NumberIterator_short__void__next(ptr nonnull %shortIterator) #2
  %14 = call i1 @_mf__NumberIterator_short__bool__isValid(ptr nonnull %shortIterator) #2
  br i1 %14, label %foreach.body.L5, label %foreach.exit.L5

foreach.exit.L5:                                  ; preds = %foreach.tail.L5, %0
  %15 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  ret i32 0
}

declare %__short__NumberIterator__short_short_short @"_f__void__NumberIterator<short>__range__short_short"(i16, i16) local_unnamed_addr

declare i1 @_mf__NumberIterator_short__bool__isValid(ptr) local_unnamed_addr

declare ptr @_mf__NumberIterator_short__shortref__get(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare %__long__NumberIterator__long_long_long @"_f__void__NumberIterator<long>__range__long_long"(i64, i64) local_unnamed_addr

declare i1 @_mf__NumberIterator_long__bool__isValid(ptr) local_unnamed_addr

declare ptr @_mf__NumberIterator_long__longref__get(ptr) local_unnamed_addr

declare void @_mp__NumberIterator_short__void__next(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
