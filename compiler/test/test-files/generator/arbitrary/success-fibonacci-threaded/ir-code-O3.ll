; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [36 x i8] c"Thread %d returned with result: %d\0A\00", align 1

; Function Attrs: nofree nosync nounwind readnone
define internal fastcc i32 @"fib(int)"(i32 %0) unnamed_addr #0 {
entry:
  %1 = icmp slt i32 %0, 3
  br i1 %1, label %common.ret, label %if.end

common.ret.loopexit:                              ; preds = %if.end
  %phi.bo = add i32 %5, 1
  br label %common.ret

common.ret:                                       ; preds = %common.ret.loopexit, %entry
  %accumulator.tr.lcssa = phi i32 [ 1, %entry ], [ %phi.bo, %common.ret.loopexit ]
  ret i32 %accumulator.tr.lcssa

if.end:                                           ; preds = %entry, %if.end
  %.tr12 = phi i32 [ %4, %if.end ], [ %0, %entry ]
  %accumulator.tr11 = phi i32 [ %5, %if.end ], [ 0, %entry ]
  %2 = add nsw i32 %.tr12, -1
  %3 = tail call fastcc i32 @"fib(int)"(i32 %2)
  %4 = add nsw i32 %.tr12, -2
  %5 = add i32 %3, %accumulator.tr11
  %6 = icmp ult i32 %.tr12, 5
  br i1 %6, label %common.ret.loopexit, label %if.end
}

define i32 @main() local_unnamed_addr {
entry:
  %threads = alloca [10 x i8*], align 8
  %i = alloca i32, align 4
  %0 = alloca { i32* }, align 8
  %1 = getelementptr inbounds { i32* }, { i32* }* %0, i64 0, i32 0
  %2 = bitcast { i32* }* %0 to i8*
  store i32 0, i32* %i, align 4
  br label %for

for:                                              ; preds = %entry, %for
  store i32* %i, i32** %1, align 8
  %3 = alloca i8*, align 8
  %4 = call i32 @pthread_create(i8** nonnull %3, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %2)
  %5 = load i8*, i8** %3, align 8
  %6 = load i32, i32* %i, align 4
  %7 = sext i32 %6 to i64
  %8 = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 %7
  store i8* %5, i8** %8, align 8
  %9 = add i32 %6, 1
  store i32 %9, i32* %i, align 4
  %10 = icmp slt i32 %9, 10
  br i1 %10, label %for, label %for.end

for.end:                                          ; preds = %for
  %.fca.9.insert.fca.9.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 9
  %.fca.9.insert.fca.8.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 8
  %.fca.9.insert.fca.7.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 7
  %.fca.9.insert.fca.6.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 6
  %.fca.9.insert.fca.5.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 5
  %.fca.9.insert.fca.4.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 4
  %.fca.9.insert.fca.3.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 3
  %.fca.9.insert.fca.2.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 2
  %.fca.9.insert.fca.1.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 1
  %.fca.9.insert.fca.0.gep = getelementptr inbounds [10 x i8*], [10 x i8*]* %threads, i64 0, i64 0
  %11 = load i8*, i8** %.fca.9.insert.fca.0.gep, align 8
  %12 = call i32 @pthread_join(i8* %11, i8** null)
  %13 = load i8*, i8** %.fca.9.insert.fca.1.gep, align 8
  %14 = call i32 @pthread_join(i8* %13, i8** null)
  %15 = load i8*, i8** %.fca.9.insert.fca.2.gep, align 8
  %16 = call i32 @pthread_join(i8* %15, i8** null)
  %17 = load i8*, i8** %.fca.9.insert.fca.3.gep, align 8
  %18 = call i32 @pthread_join(i8* %17, i8** null)
  %19 = load i8*, i8** %.fca.9.insert.fca.4.gep, align 8
  %20 = call i32 @pthread_join(i8* %19, i8** null)
  %21 = load i8*, i8** %.fca.9.insert.fca.5.gep, align 8
  %22 = call i32 @pthread_join(i8* %21, i8** null)
  %23 = load i8*, i8** %.fca.9.insert.fca.6.gep, align 8
  %24 = call i32 @pthread_join(i8* %23, i8** null)
  %25 = load i8*, i8** %.fca.9.insert.fca.7.gep, align 8
  %26 = call i32 @pthread_join(i8* %25, i8** null)
  %27 = load i8*, i8** %.fca.9.insert.fca.8.gep, align 8
  %28 = call i32 @pthread_join(i8* %27, i8** null)
  %29 = load i8*, i8** %.fca.9.insert.fca.9.gep, align 8
  %30 = call i32 @pthread_join(i8* %29, i8** null)
  ret i32 0
}

; Function Attrs: nofree nounwind
define internal noalias i8* @_thread0(i8* nocapture readonly %0) #1 {
entry:
  %1 = bitcast i8* %0 to i32**
  %2 = load i32*, i32** %1, align 8
  %3 = tail call fastcc i32 @"fib(int)"(i32 46)
  %4 = load i32, i32* %2, align 4
  %5 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([36 x i8], [36 x i8]* @0, i64 0, i64 0), i32 %4, i32 %3)
  ret i8* null
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #1

declare i32 @pthread_create(i8**, i8*, i8* (i8*)*, i8*) local_unnamed_addr

declare i32 @pthread_join(i8*, i8**) local_unnamed_addr

attributes #0 = { nofree nosync nounwind readnone }
attributes #1 = { nofree nounwind }
