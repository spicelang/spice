; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [24 x i8] c"Starting thread %d ...\0A\00", align 1
@str = private unnamed_addr constant [21 x i8] c"Starting threads ...\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"Hello from original\00", align 1
@str.2 = private unnamed_addr constant [22 x i8] c"Hello from the thread\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

define i32 @main() local_unnamed_addr {
entry:
  %i = alloca i32, align 4
  %0 = alloca { i32* }, align 8
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([21 x i8], [21 x i8]* @str, i64 0, i64 0))
  store i32 1, i32* %i, align 4
  %1 = getelementptr inbounds { i32* }, { i32* }* %0, i64 0, i32 0
  %2 = bitcast { i32* }* %0 to i8*
  br label %for

for:                                              ; preds = %entry, %for
  %storemerge6 = phi i32 [ 1, %entry ], [ %7, %for ]
  %3 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i32 %storemerge6)
  store i32* %i, i32** %1, align 8
  %4 = alloca i32, align 4
  %5 = call i32 @pthread_create(i32* nonnull %4, i8* null, i8* (i8*)* nonnull @_thread0, i8* nonnull %2)
  %6 = load i32, i32* %i, align 4
  %7 = add i32 %6, 1
  store i32 %7, i32* %i, align 4
  %8 = icmp slt i32 %7, 9
  br i1 %8, label %for, label %for.end

for.end:                                          ; preds = %for
  %9 = call i32 @usleep(i32 1000000)
  %puts5 = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i64 0, i64 0))
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

define internal noalias i8* @_thread0(i8* nocapture readonly %0) {
entry:
  %1 = bitcast i8* %0 to i32**
  %2 = load i32*, i32** %1, align 8
  %3 = load i32, i32* %2, align 4
  %4 = mul i32 %3, 100000
  %5 = tail call i32 @usleep(i32 %4)
  %puts = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([22 x i8], [22 x i8]* @str.2, i64 0, i64 0))
  ret i8* null
}

declare i32 @pthread_create(i32*, i8*, i8* (i8*)*, i8*) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
