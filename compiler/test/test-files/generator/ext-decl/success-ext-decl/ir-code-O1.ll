; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [11 x i8] c"./test-dir\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"Done.\00", align 1
@str = private unnamed_addr constant [17 x i8] c"Creating dir ...\00", align 1
@str.1 = private unnamed_addr constant [17 x i8] c"Deleting dir ...\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @mkdir(i8* nocapture noundef readonly, i32 noundef) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @rmdir(i8* nocapture noundef readonly) local_unnamed_addr #0

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  %puts = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([17 x i8], [17 x i8]* @str, i64 0, i64 0))
  %0 = call i32 @mkdir(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i64 0, i64 0), i32 511)
  %puts2 = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([17 x i8], [17 x i8]* @str.1, i64 0, i64 0))
  %1 = call i32 @rmdir(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i64 0, i64 0))
  %2 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([6 x i8], [6 x i8]* @1, i64 0, i64 0))
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
