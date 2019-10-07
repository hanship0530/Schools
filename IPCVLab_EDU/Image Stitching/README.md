# Image Stitching

## How to
1. Use Moravec detector to find keypoints.     
Build a descriptor to describe each point in an image.       

2. Compare two sets of descriptors coming from two different images and find      matching keypoints.      
3. Given a list of matching keypoints, use least-squares method to find the       affine transformation matrix that maps points in one image to another.      

4. Use RANSAC to give a more robust estimate of affine transformation matrix.             
5. Given the transformation matrix, use it to transform the second image and overlay it on the first image, forming a panorama.         
Implement a different descriptor (HOG descriptor) and get another stitching result.     

<img align="left" width="25%" src="girls_1.jpg"><img align="center" width="25%" src="girls_2.jpg"><img align="right" width="40%" src="result.png">
## Ref   
* https://github.com/mikucy/CS131    
* http://vision.stanford.edu/teaching/cs131_fall1415/schedule.html    
* https://github.com/raytroop/CS131_fall2017_Stanford-Solutions    
* https://shenxiaohai.me/2018/09/07/cs131-homework3/    
* https://www.cs.cmu.edu/~cil/v-source.html      
